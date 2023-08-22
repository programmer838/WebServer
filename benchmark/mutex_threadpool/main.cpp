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

    thread_pool pool(2);
    pool.push_task(f1);
    pool.push_task(f2);
    pool.push_task(f3);
    pool.push_task(f4);
    pool.push_task(f5);
    pool.push_task(f2);

    return 0;
}