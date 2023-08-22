#pragma once

#include <memory>
#include <cstddef>
#include <stdexcept>

template <typename T>
class ring_buffer {

public:
    ring_buffer(std::size_t k) : size(k), count(0), head(0), tail(-1) {
        queue = std::make_unique<T[]>(k);
    }

    thread_pool(const thread_pool& p) = delete;           // Disable copy constructor
    thread_pool(thread_pool&& p) = delete;                // Disable move constructor
    thread_pool& operator=(const thread_pool&) = delete;  // Disable copy assignment operator
    thread_pool& operator=(const thread_pool&&) = delete; // Disable move assignment operator

    bool push(T value) {
         
         if (full())
             return false;
        
         tail = (tail + 1) % size;
         queue[tail] = value;
         count++;

         return true;
    }
    
    bool pop() {
        
         if (empty())
             return false;

         head = (head + 1) % size;
         count--;
         
         return true;
    }
    
    T front() {
        if (empty())
	    throw std::runtime_error("ring_buffer Runtime Error: failed to retrieve Front() because buffer is empty.\n"); 
	return queue[head];
    }
    
    T rear() {
        if (empty()) 
	    throw std::runtime_error("ring_buffer Runtime Error: failed to retrieve Rear() because buffer is empty.\n"); 
        return queue[tail];
    }
    
    bool empty() {
         return count == 0 ? true : false;
    }
    
    bool full() {
         return count == size ? true : false;
    }

private:
       std::unique_ptr<T[]> queue;  
       std::size_t size, count, head, tail;
};
