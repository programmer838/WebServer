#define CATCH_CONFIG_MAIN

#include "../../include/thread_pool/thread_pool.hpp"

#include <catch2/catch.hpp>
#include <atomic>
#include <thread>

TEST_CASE("Test single push functionality") {

    std::atomic<uint64_t> variable{0};   
    auto f = [&variable](){variable++;};

    thread_pool<6, 64> tp;
    tp.push_task(f);

    while (variable.load() != 1) {}
   
    REQUIRE(variable.load() == 1);
    tp.destroy_thread_pool();
}


TEST_CASE("Test large push to fill buffers (within limits)") {

     std::atomic<uint64_t> variable{0};
     auto f = [&variable](){variable++;};

     thread_pool<6, 64> tp;

     for (int i = 1; i <= 384; i++)
          tp.push_task(f);

     while (variable.load() != 384) {}

     REQUIRE(variable.load() == 384);
     tp.destroy_thread_pool();
}


TEST_CASE("Test large push to fill buffers (exceed limits)") {

     std::atomic<uint64_t> variable{0};
     auto f = [&variable](){variable++;};

     thread_pool<6, 1048576> tp;

     for (int i = 1; i <= 6291456; i++)
          tp.push_task(f);

     while (variable.load() != 6291456) {}

     REQUIRE(variable.load() == 6291456);
     tp.destroy_thread_pool();     
}


TEST_CASE("Test push till full, pop till empty with different values") {

     std::atomic<uint64_t> variable{0};
     auto f = [&variable](int i){variable.fetch_add(i);};

     thread_pool<6, 256> tp;

     for (int i = 1; i <= 256; i++) {
	  tp.push_task(f, i);
     }

     while (variable.load() != 32896) {}

     REQUIRE(variable.load() == 32896);
     tp.destroy_thread_pool();     
}


TEST_CASE("Test large push") {

    std::atomic<uint64_t> variable{0};
    auto f = [&variable](int i){variable.fetch_add(i);};

    thread_pool<6, 65536> tp;

    for (int i = 1; i <= 65536; i++)
         tp.push_task(f, i);

    while (variable.load() != 2147516416) {}
         
    REQUIRE(variable == 2147516416);
    tp.destroy_thread_pool();   
}


TEST_CASE("Test push data functionality at specific SPSC index") {


    std::atomic<uint64_t> variable{0};
    auto f = [&variable](int i){variable.fetch_add(i);};

    thread_pool<6, 65536> tp;

    for (int i = 1; i <= 65536; i++)
         tp.push_task(static_cast<uint64_t>(0), f, i);

    while (variable.load() != 2147516416) {}

    REQUIRE(variable == 2147516416);
    tp.destroy_thread_pool();
}
