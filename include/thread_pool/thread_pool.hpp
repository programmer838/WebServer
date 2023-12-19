#pragma once

#include <atomic>
#include <thread>
#include <functional>
#include "ring_buffer.hpp"
#include "inplace_function.h"

template <uint64_t threads, uint64_t task_pool_size>
class alignas(64) thread_pool {

    static_assert((task_pool_size >= 2) && ((task_pool_size & (task_pool_size-1)) == 0), "Task pool size should be a power of 2.");

public:
    thread_pool() noexcept : round_robin(0)
    {
        for (unsigned int i = 0; i < threads; i++) {
             pool[i] = std::thread(&thread_pool::do_work, this, i);
             task_queue[i].stop.store(false, std::memory_order_release);
        }
    }

    thread_pool(const thread_pool& p) = delete;
    thread_pool(thread_pool&& p) = delete;
    thread_pool& operator=(const thread_pool&) = delete;
    thread_pool& operator=(const thread_pool&&) = delete;

    template <typename Callable, typename... Args>
    void push_task(Callable&& function, Args&&... args) noexcept {

         auto f = [function = std::forward<Callable>(function), ...args = std::forward<Args>(args)](){ function(args...); };

         int current_index = round_robin.load(std::memory_order_relaxed);
         round_robin.store(((current_index + 1) % threads), std::memory_order_release);

         while (!task_queue[current_index].rb.reserve_and_push(std::move(f))) {}
    }

    template <typename Callable, typename... Args>
    void push_task(uint64_t id, Callable&& function, Args&&... args) noexcept {

         if (id >= threads) return;
         auto f = [function = std::forward<Callable>(function), ...args = std::forward<Args>(args)](){ function(args...); };
         task_queue[id].rb.reserve_and_push(std::move(f));
    }

    void destroy_thread_pool() noexcept {

        for (int i = 0; i < threads; i++)
             task_queue[i].stop.store(true, std::memory_order_relaxed);

        for (int i = 0; i < threads; i++) {
             if (pool[i].joinable())
                 pool[i].join();
        }
    }

    unsigned long get_task_queue_size() const noexcept {
        return task_pool_size;
    }

    uint64_t get_number_of_threads() const noexcept {
        return threads;
    }

    ~thread_pool() {
        destroy_thread_pool();
    }

    void do_work(uint64_t id) noexcept {

         stdext::inplace_function<void()> return_function;

         while (!task_queue[id].stop.load(std::memory_order_acquire)) {
                if (task_queue[id].rb.try_pop(return_function))
                    return_function();
         }
    }
      
private:
    struct alignas(64) buffer_slot {
        ring_buffer<stdext::inplace_function<void()>, task_pool_size> rb;
        std::atomic<bool> stop;
    };

    std::atomic<uint64_t> round_robin;
    buffer_slot task_queue[threads];
    std::thread pool[threads];
};
