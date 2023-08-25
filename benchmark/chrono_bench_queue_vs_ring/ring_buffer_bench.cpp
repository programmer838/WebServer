#include <chrono>
#include <queue>
#include <iostream>
#include <vector>
#include "../../ring_buffer.hpp" 

void BM_RingBuffer() {
  ring_buffer<int> buffer(1000);

  // Start timing
  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < 1000; ++i) {
    buffer.push(i);
  }
  for (int i = 0; i < 1000; ++i) {
    buffer.pop();
  }

  // End timing
  auto end = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  std::cout << "Ring buffer time: " << duration << " ns\n";
}

int main() {
  BM_RingBuffer();
  return 0;
}
