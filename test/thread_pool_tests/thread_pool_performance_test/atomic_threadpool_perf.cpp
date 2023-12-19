#include <cmath>
#include <iostream>
#include <atomic>
#include "../../../include/thread_pool/thread_pool.hpp"

int main() {
 
    thread_pool<6, 32768> tp;
    std::atomic<double> result{0.0};

    auto f = [&result](){
         for (int i = 0; i < 100; i++)
              result.fetch_add(std::sqrt(i));
    };

    for (int i = 0; i < 1000; i++)
         tp.push_task(f);

    while (static_cast<int>(result.load()) != 661462) {}
    tp.destroy_thread_pool();

    return 0;
}
