#pragma once

#include <memory>
#include <utility>
#include <cstddef>
#include <stdexcept>

template <typename T>
class ring_buffer {

public:
    std::unique_ptr<T[]> queue;  
    std::size_t size, count, head, tail;

    ring_buffer(std::size_t k) : size(k), count(0), head(0), tail(-1) {
        queue = std::make_unique<T[]>(k);
    }
 
    ring_buffer(ring_buffer&& p) noexcept {                        // Move constructor
	
	this->queue = std::move(p.queue);
	this->size  = p.size;
	this->count = p.count;
	this->head  = p.head;
        this->tail  = p.tail;

	p.size  = 0;
        p.count = 0;
        p.head  = 0;
        p.tail  = 0;	     
    }

    ring_buffer& operator=(ring_buffer&& p) noexcept {         // Move assignment operator
    	if (this == &p) return *this;

	this->queue = std::move(p.queue);
        this->size  = p.size;
        this->count = p.count;
        this->head  = p.head;
        this->tail  = p.tail;

        p.size  = 0;
        p.count = 0;
        p.head  = 0;
        p.tail  = 0;

	return *this;
    }

    ring_buffer(const ring_buffer& p) = delete;           // Disable copy constructor
    ring_buffer& operator=(const ring_buffer&) = delete;  // Disable copy assignment operator

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
};
