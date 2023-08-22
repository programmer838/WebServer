#include <benchmark/benchmark.h>
#include "thread_pool.hpp" // Include your thread pool header
#include <cmath>

void f1() {
    double result = 0.0;
    for (int i = 0; i < 1000000; ++i) result += std::pow(1.23, 4.56);
    if (result < 0) std::cout << "This will never print\n";
}

void f2() {
   double result = 0.0;
   for (int i = 0; i < 1000000; ++i) result += std::pow(1.50, 3.24);
   if (result < 0) std::cout << "This will never print\n";
}

void f3() {
    double result = 0.0;
    for (int i = 0; i < 1000000; ++i) result += std::pow(1.70, 5.33);
    if (result < 0) std::cout << "This will never print\n";
}

void f4() {
    double result = 0.0;
    for (int i = 0; i < 1000000; ++i) result += std::pow(1.99, 5.21);
    if (result < 0) std::cout << "This will never print\n";
}

void f5() {
    double result = 0.0;
    for (int i = 0; i < 1000000; ++i) result += std::pow(2.47, 4.47);
    if (result < 0) std::cout << "This will never print\n";
}

static void BM_ThreadPool(benchmark::State& state) {
  for (auto _ : state) {
    thread_pool pool(6);
    pool.push_task(f1);
    pool.push_task(f2);
    pool.push_task(f3);
    pool.push_task(f4);
    pool.push_task(f5);
  }
}

BENCHMARK(BM_ThreadPool);

BENCHMARK_MAIN();

