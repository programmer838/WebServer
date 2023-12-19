#pragma once

/*
 * The caching mechanism implemented in this ring buffer is inspired by the AF_XDP socket queue
 * implemented in the Linux Kernel, specifically as seen in:
 * https://github.com/torvalds/linux/blob/master/net/xdp/xsk_queue.h
 */

#include <atomic>

template <typename T, uint64_t ring_buffer_size>
class ring_buffer {

    static_assert((ring_buffer_size >= 2) && ((ring_buffer_size & (ring_buffer_size - 1)) == 0));

public:
    ring_buffer() noexcept : head(0), tail(0), cached_head(0), cached_tail(0) {}

    bool reserve_and_push(T&& value) noexcept {
	
         const uint64_t current_head = head.load(std::memory_order_acquire);
         const uint64_t next_head = (current_head + 1) & (ring_buffer_size - 1);

         if (next_head == cached_tail) [[unlikely]] {
             if ((cached_tail = tail.load(std::memory_order_acquire)) == next_head)
                 return false;
         }

         queue[current_head] = value;
         head.store(next_head, std::memory_order_release);

         return true;
    }

    bool try_pop(T& func) noexcept {

         const uint64_t current_tail = tail.load(std::memory_order_acquire);
         const uint64_t next_tail = (current_tail + 1) & (ring_buffer_size - 1);

         if (current_tail == cached_head) [[unlikely]] {
             if ((cached_head = head.load(std::memory_order_acquire)) == current_tail)
                 return false;
         }

         func = queue[current_tail];
         tail.store(next_tail, std::memory_order_release);

         return true;
    }

    ~ring_buffer() {
         delete[] queue;
    }

    T *queue = new T[ring_buffer_size];

    alignas(64) std::atomic<uint64_t> head;
    alignas(64) std::atomic<uint64_t> tail;
    alignas(64) uint64_t cached_head;
    alignas(64) uint64_t cached_tail;    
};
