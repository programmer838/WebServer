#include <condition_variable>
#include <functional>
#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <array>
#include <queue>
#include <mutex>

#include <sys/socket.h>
#include <liburing.h>

class thread_pool {

public:
    thread_pool() : pool(std::make_unique<std::thread[]>(std::thread::hardware_concurrency())) {

    }

    thread_pool(const thread_pool& p) = delete;           // Disable copy constructor
    thread_pool(thread_pool&& p) = delete;                // Disable move constructor
    thread_pool& operator=(const thread_pool&) = delete;  // Disable copy assignment operator
    thread_pool& operator=(const thread_pool&&) = delete; // Disable move assignment operator


    void push_task(std::function<void()> function) {

         {                                                            
            const std::scoped_lock<std::mutex> scoped_lock(mutex);    // Aquire mutex in block scope
            task_queue.push(function);                             // Add task to task queue
         }
                                                                  // At this point RAII lock has released mutex since we are out of block scope
         conditional_variable.notify_one();                       // Notify/wakeup thread that task is available
    }

private:
    std::queue<std::function<void()>> task_queue; 
    std::unique_ptr<std::thread[]> pool;          
    std::condition_variable conditional_variable;
    std::mutex mutex;   
};

int main(int argc, char **argv)
{
    struct io_uring ring;
    io_uring_queue_init(10, &ring, 0);

    return 0;
}