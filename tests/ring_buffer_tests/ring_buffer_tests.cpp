#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "ring_buffer.hpp"
#include <functional>
#include "inplace_function.h"

TEST_CASE("Test single push and single pop") {

     int variable = 0;
     auto f = [&variable](){variable++;};

     ring_buffer<stdext::inplace_function<void()>, 2> rb;

     rb.reserve_and_push(f);
     rb.try_pop_and_run_job();

     REQUIRE(variable == 1);
}

TEST_CASE("Test pop on empty ring buffer") {

     ring_buffer<stdext::inplace_function<void()>, 256> rb;

     for (int i = 1; i <= 256; i++)
          REQUIRE(rb.try_pop_and_run_job() == false);
}

TEST_CASE("Test push till full") {

     int variable = 0;
     auto f = [&variable](){variable++;};

     ring_buffer<stdext::inplace_function<void()>, 256> rb;

     for (int i = 1; i <= 256; i++)
          rb.reserve_and_push(f);

     for (int i = 1; i <= 256; i++) 
          rb.try_pop_and_run_job();

     REQUIRE(variable == 256);
}

TEST_CASE("Test push after full, pop after empty") {

     int variable = 0;
     auto f = [&variable](){variable++;};

     ring_buffer<stdext::inplace_function<void()>, 256> rb;

     for (int i = 1; i <= 500; i++)
          rb.reserve_and_push(f);

     for (int i = 1; i <= 500; i++) 
          rb.try_pop_and_run_job();

     REQUIRE(variable == 256);
}


TEST_CASE("Test push till full, pop till empty, push till full, pop till empty") {

     int variable = 0;
     auto f = [&variable](){variable++;};

     ring_buffer<stdext::inplace_function<void()>, 256> rb;

     for (int i = 1; i <= 256; i++)
          rb.reserve_and_push(f);

     for (int i = 1; i <= 256; i++)
          rb.try_pop_and_run_job();

     for (int i = 1; i <= 256; i++)
          rb.reserve_and_push(f);

     for (int i = 1; i <= 256; i++)
          rb.try_pop_and_run_job();

     REQUIRE(variable == 512); 
}

TEST_CASE("Test values before and after pushing and popping") {

     int variable = 0;
     auto f = [&variable](){variable++;};

     ring_buffer<stdext::inplace_function<void()>, 256> rb;

     for (int i = 1; i <= 256; i++) {
          rb.reserve_and_push(f);
	  REQUIRE(variable == i-1);
          rb.try_pop_and_run_job();
          REQUIRE(variable == i);          
     }

     REQUIRE(variable == 256);
}
