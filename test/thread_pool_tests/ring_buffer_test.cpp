#define CATCH_CONFIG_MAIN

#include "../../include/thread_pool/ring_buffer.hpp"
#include "../../include/thread_pool/inplace_function.h"

#include <catch2/catch.hpp>
#include <thread>

TEST_CASE("Single Threaded --- Test single push and single pop") {

     int variable = 0;
     auto f = [&variable](){variable++;};
     stdext::inplace_function<void()> return_function;     

     ring_buffer<stdext::inplace_function<void()>, 32> rb;

     rb.reserve_and_push(f);
     rb.try_pop(return_function);
     return_function();

     REQUIRE(variable == 1);
}


TEST_CASE("Single Threaded --- Test pop on empty ring buffer") {

     ring_buffer<stdext::inplace_function<void()>, 256> rb;
     stdext::inplace_function<void()> return_function;

     for (int i = 1; i <= 256; i++)
          REQUIRE(rb.try_pop(return_function) == false);
}


TEST_CASE("Single Threaded --- Test push till full") {

     int variable = 0;
     auto f = [&variable](){variable++;};
     stdext::inplace_function<void()> return_function;    
 
     ring_buffer<stdext::inplace_function<void()>, 256> rb;

     for (int i = 1; i <= 256; i++)
          rb.reserve_and_push(f);

     for (int i = 1; i <= 256; i++) {
          if (rb.try_pop(return_function))
    	      return_function();
     }

     REQUIRE(variable == 255);
}


TEST_CASE("Single Threaded --- Test push after full, pop after empty") {

     int variable = 0;
     auto f = [&variable](){variable++;};
     stdext::inplace_function<void()> return_function;   
  
     ring_buffer<stdext::inplace_function<void()>, 256> rb;

     for (int i = 1; i <= 500; i++)
          rb.reserve_and_push(f);

     for (int i = 1; i <= 500; i++) {
          if (rb.try_pop(return_function))
     	      return_function();
     }

     REQUIRE(variable == 255);
}

TEST_CASE("Single Threaded --- Test push till full, pop till empty, push till full, pop till empty") {

     int variable = 0;
     auto f = [&variable](){variable++;};
     stdext::inplace_function<void()> return_function;   

     ring_buffer<stdext::inplace_function<void()>, 256> rb;

     for (int i = 1; i <= 256; i++)
          rb.reserve_and_push(f);

     for (int i = 1; i <= 256; i++) {
          rb.try_pop(return_function);
   	  return_function(); 
     }

     for (int i = 1; i <= 256; i++)
          rb.reserve_and_push(f);

     for (int i = 1; i <= 256; i++) {
          rb.try_pop(return_function);
	  return_function();
     }

     REQUIRE(variable == 512); 
}


TEST_CASE("Single Threaded --- Test values before and after pushing and popping") {

     int variable = 0;
     auto f = [&variable](){variable++;};
     stdext::inplace_function<void()> return_function;  
 
     ring_buffer<stdext::inplace_function<void()>, 256> rb;

     for (int i = 1; i <= 256; i++) {
          rb.reserve_and_push(f);
	  REQUIRE(variable == i-1);
          rb.try_pop(return_function);
          return_function();
          REQUIRE(variable == i);          
     }

     REQUIRE(variable == 256);
}


TEST_CASE("Multi Threaded --- Test push 100 times and pop 100 times") {

     int variable = 0;
     auto increment_variable = [&variable](){variable++;};

     stdext::inplace_function<void()> return_function;
     ring_buffer<stdext::inplace_function<void()>, 256> rb;
      
     auto producer = [&rb, &increment_variable](){

	  for (int i = 1; i <= 100; i++)
               rb.reserve_and_push(increment_variable);
     }; 

     auto consumer = [&rb, &return_function](){
    
          int i = 1;

          while (i <= 100) {
               if (rb.try_pop(return_function)) {
                   return_function();
                   i++;
               }
	  } 
     }; 

     std::thread prod(producer);
     std::thread cons(consumer);

     prod.join();
     cons.join();

     REQUIRE(variable == 100);
}

TEST_CASE("Multi Threaded --- Test push 1 and pop 1") {

     int variable = 0;
     auto increment_variable = [&variable](){variable++;};

     stdext::inplace_function<void()> return_function;
     ring_buffer<stdext::inplace_function<void()>, 256> rb;

     auto producer = [&rb, &increment_variable](){

          rb.reserve_and_push(increment_variable);
     };

     auto consumer = [&rb, &return_function](){
	
          int i = 1;	
 
          while (i <= 1) {
               if (rb.try_pop(return_function)) {
                   return_function();
                   i++;
               }
 	 }
     };

     std::thread prod(producer);
     std::thread cons(consumer);

     prod.join();
     cons.join();

     REQUIRE(variable == 1);
}


TEST_CASE("Multi Threaded --- Test pushing 16,000,000 times and popping 16,000,000 times") {

     int variable = 0;
     auto increment_variable = [&variable](){variable++;};

     stdext::inplace_function<void()> return_function;
     ring_buffer<stdext::inplace_function<void()>, 16777216> rb;

     auto producer = [&rb, &increment_variable](){

	  for (int i = 1; i <= 16000000; i++) 	
               rb.reserve_and_push(increment_variable);
     };

     auto consumer = [&rb, &return_function](){

          int i = 1;

          while (i <= 16000000) {
               if (rb.try_pop(return_function)) {
                   return_function();
                   i++;
               }
         }
     };

     std::thread prod(producer);
     std::thread cons(consumer);

     prod.join();
     cons.join();

     REQUIRE(variable == 16000000);
}


TEST_CASE("Multi Threaded --- Test push till full, pop till empty, push till full, pop till empty") {

     int variable = 0;
     auto increment_variable = [&variable](){variable++;};

     stdext::inplace_function<void()> return_function;
     ring_buffer<stdext::inplace_function<void()>, 1048576> rb;

     auto producer = [&rb, &increment_variable](){

          for (int i = 1; i <= 1048576; i++)
               rb.reserve_and_push(increment_variable);
     };

     auto consumer = [&rb, &return_function](){

          int i = 1;

          while (i <= 1048575) {
               if (rb.try_pop(return_function)) {
                   return_function();
                   i++;
               }
         }
     };

     std::thread prod1(producer);
     std::thread cons1(consumer);

     prod1.join();
     cons1.join();

     std::thread prod2(producer);
     std::thread cons2(consumer);
 
     prod2.join();
     cons2.join();

     REQUIRE(variable == 2097150);
}


TEST_CASE("Multi Threaded --- Test push till full, pop till empty with different values") {

     uint64_t variable = 0;

     stdext::inplace_function<void()> return_function;
     ring_buffer<stdext::inplace_function<void()>, 65536> rb;

     auto producer = [&rb, &variable](){

          for (int i = 1; i <= 65536; i++) {
               auto increment_variable = [&variable, i](){variable += i;};
	       rb.reserve_and_push(increment_variable);
	  }
     };

     auto consumer = [&rb, &return_function](){

          int i = 1;

          while (i <= 65536) {
               if (rb.try_pop(return_function)) {
                   return_function();
                   i++;
               }
         }
     };

     std::thread prod1(producer);
     std::thread cons1(consumer);

     prod1.join();
     cons1.join();

     REQUIRE(variable == 2147516416);
}
