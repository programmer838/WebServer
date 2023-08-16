#include <condition_variable>
#include <functional>
#include <iostream>
#include <vector>
#include <memory>
#include <atomic>
#include <thread>
#include <array>
#include <queue>
#include <mutex>

#include <sys/socket.h>
#include <liburing.h>

class thread_pool {

public:
    thread_pool(uint32_t threads) : number_of_threads(threads), tasks_processed(0), stop(false)
    {
        pool = std::make_unique<std::thread[]>(number_of_threads);

        for (unsigned int i = 0; i < number_of_threads; i++) {
             pool.get()[i] = std::thread(&thread_pool::do_work, this); 
        }
    }

    thread_pool(const thread_pool& p) = delete;           // Disable copy constructor
    thread_pool(thread_pool&& p) = delete;                // Disable move constructor
    thread_pool& operator=(const thread_pool&) = delete;  // Disable copy assignment operator
    thread_pool& operator=(const thread_pool&&) = delete; // Disable move assignment operator

    template <typename Callable, typename... Args>
    void push_task(Callable&& function, Args&&... args) { // Perfect forwarding

         {
            const std::scoped_lock scoped_lock(mutex);                                                    // Aquire mutex in block scope
            task_queue.push(std::bind(std::forward<Callable>(function), std::forward<Args>(args)...));    // Add task to task queue
         }
                                                                  // At this point RAII lock has released mutex since we are out of block scope
         conditional_variable.notify_one();                       // Notify/wakeup thread that task is available
    }

    void destroy_thread_pool() {

        stop = true;

        conditional_variable.notify_all();     // Wake up every thread that is sleeping, this causes the threads to exit the while loop if there is no more work to do

        for (int i = 0; i < number_of_threads; i++) { 
             if (pool.get()[i].joinable()) {  
                 pool.get()[i].join();         // To terminate/kill each thread we need to join it, aka make the calling thread (main thread) wait for it to complete its task
                                            // And eventually terminate, this means we must wait for every thread doing work to finish and doing .join() allows us to do this
             }
        }
    }

    inline unsigned long get_task_queue_size() { // Return size of current task queue for logging purposes
        std::scoped_lock scoped_lock(mutex);
        return task_queue.size();
    }

    uint64_t get_number_tasks_processed() { // Return total tasks processed for logging purposes
        return tasks_processed;    
    }

private:
    std::queue<std::function<void()>> task_queue; 
    std::unique_ptr<std::thread[]> pool;          
    std::condition_variable conditional_variable;
    std::mutex mutex;   
    uint32_t number_of_threads;
    std::atomic<uint64_t> tasks_processed;
    std::atomic<bool> stop;

    void do_work() {

        std::function<void()> task;

        while (stop == false) {
            
            {
                std::unique_lock unique_lock(mutex);
                conditional_variable.wait(unique_lock, [&](){return stop || !task_queue.empty();}); // Handle spurious wakeups and wait if condition is false
                if (stop && task_queue.empty()) return;                                                      // Upon termination only terminate after the remaining tasks have been handled
                task = std::move(task_queue.front());
                task_queue.pop();
            }

            task();
            ++tasks_processed;
        }
    }

    ~thread_pool() {
        destroy_thread_pool();
    }
};

int main(int argc, char **argv)
{
    struct io_uring ring;
    io_uring_queue_init(10, &ring, 0);

    return 0;
}