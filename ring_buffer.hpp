#pragma once

#include <memory>
#include <mutex>
#include <utility>
#include <stdexcept>

template <typename T>
class ring_buffer {

public:
    T *queue;  
    int ssize; 
    int count;
    int head;
    int tail;
    
    ring_buffer(int k) : ssize(k), count(0), head(0), tail(-1) {
        queue = new T[k];
    }
 
    ring_buffer(ring_buffer&& p) noexcept {                        // Move constructor
	
	this->queue = std::move(p.queue);
	this->ssize  = p.ssize;
	this->count = p.count;
	this->head  = p.head;
        this->tail  = p.tail;

	p.queue = nullptr;
	p.ssize  = 0;
        p.count = 0;
        p.head  = 0;
        p.tail  = 0;	     
    }

    ring_buffer& operator=(ring_buffer&& p) noexcept {         // Move assignment operator
    	if (this == &p) return *this;

	this->queue = std::move(p.queue);
        this->ssize  = p.ssize;
        this->count = p.count;
        this->head  = p.head;
        this->tail  = p.tail;
	
	p.queue = nullptr;
        p.ssize  = 0;
        p.count = 0;
        p.head  = 0;
        p.tail  = 0;

	return *this;
    }

    ring_buffer(const ring_buffer& p) = delete;           // Disable copy constructor
    ring_buffer& operator=(const ring_buffer&) = delete;  // Disable copy assignment operator

    bool push(T value) {
         
         if (count == ssize)
             return false;
        
         tail = (tail + 1) % ssize;
         queue[tail] = value;
         count++;

         return true;
    }
    
    bool pop() {
        
         if (count == 0)
             return false;

         head = (head + 1) % ssize;
         count--;
         
         return true;
    }

    T& front() {
        if (count == 0)
	    throw std::runtime_error("ring_buffer Runtime Error: failed to retrieve Front() because buffer is empty.\n"); 
	return queue[head];
    }

    bool empty() {
         return count == 0;
    }

    int size() {
	return count;
    }
   
    ~ring_buffer() {
	delete[] queue;
    }
};
