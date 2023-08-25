#include <chrono>
#include <queue>
#include <iostream>
#include <vector>
#include "../../ring_buffer.hpp" 

void BM_StdQueue() {
  std::queue<int> queue;

  // Start timing
  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < 1000; ++i) {
    queue.push(i);
  }
  for (int i = 0; i < 1000; ++i) {
    queue.pop();
  }

  // End timing
  auto end = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  std::cout << "std::queue time: " << duration << " ns\n";
}

int main() {
  BM_StdQueue();
  return 0;
}
