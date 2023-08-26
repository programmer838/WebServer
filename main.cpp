#include <condition_variable>
#include <functional>
#include <iostream>
#include <vector>
#include <memory>
#include <atomic>
#include <thread>
#include <chrono>
#include <array>
#include <queue>
#include <mutex>

#include <sys/socket.h>
#include <liburing.h>
#include "thread_pool.hpp"

struct alignas (64) aligned_type {
//https://www.youtube.com/watch?v=BP6NxVxDQIs
//47:56
};

void f1() {
    std::cout << "Hello from function 1\n";
}

void f2() {
    std::cout << "Hello from function 2\n";
}

void f3() {
    std::cout << "Hello from function 3\n";
}

void f4() {
    std::cout << "Hello from function 4\n";
}

void f5() {
    std::cout << "Hello from function 5\n";
}

int main(int argc, char **argv)
{
    struct io_uring ring;
    io_uring_queue_init(10, &ring, 0);

    thread_pool pool(6,1);

    auto start_time = std::chrono::high_resolution_clock::now();
    while(!pool.push_task(f1));
    while(!pool.push_task(f2));
    while(!pool.push_task(f3));
    while(!pool.push_task(f4));
    while(!pool.push_task(f5));
    while(!pool.push_task(f2));


    pool.destroy_thread_pool(); // Make sure to join the threads to wait for them to finish

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

    std::cout << "Time taken: " << duration << " nanoseconds" << std::endl;
    return 0;
}
