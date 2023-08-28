#pragma once

#include <mutex>
#include <thread>
#include <atomic>
#include <memory>
#include <vector>
#include <iostream>
#include <functional>
#include <condition_variable>

#include "ring_buffer.hpp"

template <uint32_t threads, uint32_t task_pool_size>
class thread_pool {

public: 
    thread_pool() : number_of_threads(threads), tasks_processed(0), stop(false)
    {
	pool = std::make_unique<std::thread[]>(number_of_threads);

        for (unsigned int i = 0; i < number_of_threads; i++)
             pool.get()[i] = std::thread(&thread_pool::do_work, this); 
    }

    thread_pool(const thread_pool& p) = delete;           // Disable copy constructor
    thread_pool(thread_pool&& p) = delete;                // Disable move constructor
    thread_pool& operator=(const thread_pool&) = delete;  // Disable copy assignment operator
    thread_pool& operator=(const thread_pool&&) = delete; // Disable move assignment operator

    template <typename Callable, typename... Args>
    bool push_task(Callable&& function, Args&&... args) { // Perfect forwarding

	    bool ret;		

            {
                 mutex.lock();	
		 auto f = [function = std::forward<Callable>(function), ...args = std::forward<Args>(args)](){ function(args...); };
		 // Use lambda instead of std::bind
		 ret = task_queue.push(f);
            	 mutex.unlock();
	    }

            conditional_variable.notify_one();                       // Notify/wakeup thread that task is available
	    return ret;	    
    }


    void destroy_thread_pool() {

        mutex.lock();
        stop = true;
        mutex.unlock();

        conditional_variable.notify_all();     // Wake up every thread that is sleeping

        for (int i = 0; i < number_of_threads; i++) { 
                if (pool.get()[i].joinable())  
                    pool.get()[i].join();         // To kill each thread we need to join it, this makes the calling thread (main thread) wait for it to 
                                                  // Complete its task and eventually terminate, this means we must wait for every thread doing work to 
                                                  // Finish and doing .join() allows us to do this
        }
    }

    unsigned long get_task_queue_size() {  // Return size of current task queue for logging purposes

        mutex.lock();
        unsigned long task_queue_size = task_queue.size();
        mutex.unlock();
        return task_queue_size;
    }

    uint64_t get_number_tasks_processed() {      // Return total tasks processed for logging purposes

        mutex.lock();
        return tasks_processed;
        mutex.unlock();    
    }

    ~thread_pool() {
        destroy_thread_pool();
    }

    void do_work() {

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

private:
    ring_buffer<std::function<void()>, task_pool_size> task_queue;
    std::unique_ptr<std::thread[]> pool;
    std::condition_variable conditional_variable;
    std::mutex mutex;
    uint32_t number_of_threads;
    std::atomic<uint64_t> tasks_processed;
    bool stop;
};
