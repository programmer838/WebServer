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
    thread_pool() : pool(std::make_unique<std::thread[]>(std::thread::hardware_concurrency())) 
                  , stop_flag(false)
    {

        /* 
            This constructor two main things:

            1) Allocate memory for X threads, I decided to keep this to std::thread::hardware_concurrency() which returns the number of CPU cores
            2) Initialise and create threads, and pass each thread a instance of the thread_pool::do_work() function which will have an infinite loop
            which will be used to allow it to keep doing tasks when they are available in the task queue (check the comments for do_work() for more
            details on it).
        */

        for (unsigned int i = 0; i < std::thread::hardware_concurrency(); i++) {
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

            /*
                Perfect forwarding in C++ allows functions to accept arguments and then forward them to another function, preserving the original 
                argument's value category (i.e., whether it's an lvalue or rvalue). This is particularly useful when writing template functions, 
                like wrappers or factories, where you want the function to be agnostic about the argument's type and value category. By preserving 
                the semantics, we can ensure that move semantics of objects are honored where applicable. This is accomplished using a combination 
                of rvalue references and the std::forward utility.

                By using perfect forwarding with "Callable" and "args" we are able to efficiently bind a function to its arguments
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
    bool stop_flag;

    void do_work() {

         while (stop_flag == false) {

         }
    }
};

int main(int argc, char **argv)
{
    struct io_uring ring;
    io_uring_queue_init(10, &ring, 0);

    return 0;
}