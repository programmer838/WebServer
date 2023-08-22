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

    bool enQueue(T value) {
         
         if (isFull())
             return false;
        
         tail = (tail + 1) % size;
         queue[tail] = value;
         count++;

         return true;
    }
    
    bool deQueue() {
        
         if (isEmpty())
             return false;

         head = (head + 1) % size;
         count--;
         
         return true;
    }
    
    T Front() {
        if (isEmpty())
	    throw std::runtime_error("ring_buffer Runtime Error: failed to retrieve Front() because buffer is empty.\n"); 
	return queue[head];
    }
    
    T Rear() {
        if (isEmpty()) 
	    throw std::runtime_error("ring_buffer Runtime Error: failed to retrieve Rear() because buffer is empty.\n"); 
        return queue[tail];
    }
    
    bool isEmpty() {
         return count == 0 ? true : false;
    }
    
    bool isFull() {
         return count == size ? true : false;
    }

private:
       std::unique_ptr<T[]> queue;  
       std::size_t size, count, head, tail;
};
