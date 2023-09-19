#pragma once

#include <atomic>

template <typename T, uint64_t ring_buffer_size>
class ring_buffer {

    static_assert((ring_buffer_size >= 2) && ((ring_buffer_size & (ring_buffer_size-1)) == 0), "Ring buffer size should be power of 2.\n");

public:
    ring_buffer(const ring_buffer& rhs) = delete;
    ring_buffer(ring_buffer&& rhs) = delete;
    ring_buffer& operator=(const ring_buffer& rhs) = delete;
    ring_buffer& operator=(ring_buffer&& rhs) = delete;

    ring_buffer() noexcept : head(0), tail(0) {
	for (int i = 0; i < ring_buffer_size; i++) {
	     queue[i].sequence_number.store(i, std::memory_order_relaxed);
	}
    }

    void reserve_and_push(T&& value) noexcept {
         uint64_t expected;
	 uint64_t current_sequence_number; 

	 while (true) {
		expected = head.load(std::memory_order_relaxed);
		current_sequence_number = queue[expected & (ring_buffer_size-1)].sequence_number;

		if (expected == current_sequence_number) {
	            if (head.compare_exchange_weak(expected, expected+1, std::memory_order_relaxed)) {
		    	break;
		    } 
		}
		else if (current_sequence_number < expected) {
		    std::cout << "full\n";
		    return;
		}
	 }
	
	 std::cout << "pushed to: " << expected << '\n';
 	 queue[expected & (ring_buffer_size-1)].data = std::move(value);
	 queue[expected & (ring_buffer_size-1)].sequence_number++;
    }

    bool try_pop_and_run_job() noexcept {
	 uint64_t expected;
	 uint64_t current_sequence_number;	
 
	 while (true) {
	        expected = tail.load(std::memory_order_relaxed);
		current_sequence_number = queue[expected & (ring_buffer_size-1)].sequence_number;

		if (expected != current_sequence_number) {
		    if (tail.compare_exchange_weak(expected, expected+1, std::memory_order_relaxed)) {
		   	 break;
		    }	
		}
		else if (expected == current_sequence_number) {
		    std::cout << "empty\n";
		    return false;
		}
	 }

	 std::cout << "popped from: " << expected << '\n';
         auto task = std::move(queue[expected & (ring_buffer_size-1)].data);
	 task();
	 queue[expected & (ring_buffer_size-1)].sequence_number.store(expected+ring_buffer_size, std::memory_order_relaxed);
	 
	 return true;	 
    }

private:

    struct buffer_slot {
	std::atomic<uint64_t> sequence_number;
	T data;
    };

    buffer_slot queue[ring_buffer_size];
    alignas(64) std::atomic<uint64_t> head;
    alignas(64) std::atomic<uint64_t> tail;
};
