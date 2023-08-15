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

    template <typename Callable, typename... Args>
    void push_task(Callable&& function, Args&&... args) { // Perfect forwarding

         {

            /*
                By using perfect forwarding with Callable and args we are able to efficiently bind a function to its arguments
                whilst maintainig the semantics and details of the arguments (such as if we want to retain the type information)
                and push them into the task queue. std::bind will essentially bind the function and its arguments and return a callable 
                object which does not return anything (void) and does not take any placeholder arguments (arguments the user provides) since
                these are provided through args when the bind is done. So in the end we get a callable object that is compatible with std::function(void()).
            */

            const std::scoped_lock scoped_lock(mutex);                                                    // Aquire mutex in block scope
            task_queue.push(std::bind(std::forward<Callable>(function), std::forward<Args>(args)...));    // Add task to task queue
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