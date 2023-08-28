#pragma once

#include <memory>
#include <mutex>
#include <utility>
#include <stdexcept>

template <typename T, uint32_t ring_buffer_size>
class ring_buffer {

public:
    T queue[ring_buffer_size];    
    int count;
    int head;
    int tail;
    
    ring_buffer() : count(0), head(0), tail(-1) {}
 
    ring_buffer(ring_buffer&& p) = delete;                // Delete Move constructor
    ring_buffer& operator=(ring_buffer&& p) = delete;     // Delete move assignment operator
    ring_buffer(const ring_buffer& p) = delete;           // Disable copy constructor
    ring_buffer& operator=(const ring_buffer&) = delete;  // Disable copy assignment operator

    bool push(T value) noexcept {
         
         if (count == ring_buffer_size)
             return false;
        
         tail = (tail + 1) % ring_buffer_size;
         queue[tail] = value;
         count++;

         return true;
    }
    
    bool pop() noexcept {
        
         if (count == 0)
             return false;

         head = (head + 1) % ring_buffer_size;
         count--;
         
         return true;
    }

    T& front() {
        if (count == 0)
	    throw std::runtime_error("ring_buffer Runtime Error: failed to retrieve Front() because buffer is empty.\n"); 
	return queue[head];
    }

    bool empty() const noexcept {
        return count == 0;
    }

    int size() const noexcept {
	return count;
    }
};
