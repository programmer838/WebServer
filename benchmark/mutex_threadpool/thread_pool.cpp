#include "thread_pool.hpp"

thread_pool::thread_pool(uint32_t threads) 
            : number_of_threads(threads), 
                tasks_processed(0), 
                stop(false)
{
    if (number_of_threads == 0) number_of_threads = 1; // Check if either the user gives 0 or if hardware_concurrency() returns 0

    pool = std::make_unique<std::thread[]>(number_of_threads);

    for (unsigned int i = 0; i < number_of_threads; i++) {
            pool.get()[i] = std::thread(&thread_pool::do_work, this); 
    }
}

void thread_pool::destroy_thread_pool() {

    mutex.lock();
    stop = true;
    mutex.unlock();

    conditional_variable.notify_all();     // Wake up every thread that is sleeping

    for (int i = 0; i < number_of_threads; i++) { 
            if (pool.get()[i].joinable()) {  
                pool.get()[i].join();         // To kill each thread we need to join it, this makes the calling thread (main thread) wait for it to 
                                            // Complete its task and eventually terminate, this means we must wait for every thread doing work to 
                                            // Finish and doing .join() allows us to do this
            }
    }
}

unsigned long thread_pool::get_task_queue_size() {  // Return size of current task queue for logging purposes

    mutex.lock();
    unsigned long task_queue_size = task_queue.size();
    mutex.unlock();
    return task_queue_size;
}

uint64_t thread_pool::get_number_tasks_processed() {      // Return total tasks processed for logging purposes

    mutex.lock();
    return tasks_processed;
    mutex.unlock();    
}

thread_pool::~thread_pool() {
    destroy_thread_pool();
}

void thread_pool::do_work() {

    std::function<void()> task;

    while (true) {

        {
            std::unique_lock unique_lock(mutex);
            conditional_variable.wait(unique_lock, [&](){return stop || !task_queue.empty();}); // Handle spurious wakeups and wait if condition is false
            if (stop && task_queue.empty()) break;                                              // Upon termination only terminate after the remaining 
                                                                                                // Tasks have been handled
            task = std::move(task_queue.front());
            task_queue.pop();
        }

        task();
        ++tasks_processed;
    }
}
