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

void printVec(std::vector<int> vec) {
     int sum = 0;
     for (int i = 0; i < vec.size(); ++i) sum += vec[i];	
     std::cout << "sum is : " << sum << '\n';
}

int main(int argc, char **argv)
{
    struct io_uring ring;
    io_uring_queue_init(10, &ring, 0);
    thread_pool<6,5> pool;

    std::vector<int> vec{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,52,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100};

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
