#pragma once

#include <condition_variable>
#include <functional>
#include <iostream>
#include <vector>
#include <memory>
#include <atomic>
#include <thread>
#include <queue>
#include <mutex>

#include "ring_buffer.hpp"

class thread_pool {

public:
    thread_pool(uint32_t threads = std::thread::hardware_concurrency(), uint32_t task_pool_size = std::thread::hardware_concurrency());

    thread_pool(const thread_pool& p) = delete;           // Disable copy constructor
    thread_pool(thread_pool&& p) = delete;                // Disable move constructor
    thread_pool& operator=(const thread_pool&) = delete;  // Disable copy assignment operator
    thread_pool& operator=(const thread_pool&&) = delete; // Disable move assignment operator

    template <typename Callable, typename... Args>
    bool push_task(Callable&& function, Args&&... args) { // Perfect forwarding

	    bool ret;		

            {
                 mutex.lock();
                 ret = task_queue.push(std::bind(std::forward<Callable>(function), std::forward<Args>(args)...));  // Add task to task queue
            	 mutex.unlock();
	    }
                                                                    // At this point RAII lock has released mutex since we are out of block scope
            conditional_variable.notify_one();                       // Notify/wakeup thread that task is available
	    return ret;	    
    }

    void destroy_thread_pool();

    unsigned long get_task_queue_size();

    uint64_t get_number_tasks_processed();

    ~thread_pool();

private:
    ring_buffer<std::function<void()>> task_queue; 
    std::unique_ptr<std::thread[]> pool;          
    std::condition_variable conditional_variable;
    std::mutex mutex;   
    uint32_t number_of_threads;
    std::atomic<uint64_t> tasks_processed;
    bool stop;

    void do_work();
};

