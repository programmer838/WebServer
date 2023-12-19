#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include "../../include/lru_cache/lru_cache.hpp"

TEST_CASE("Empty Cache Retrieval") {
   
    lru_cache<5> cache;
   
    REQUIRE(cache.get(1) == nullptr);
    REQUIRE(cache.get(2) == nullptr); 
    REQUIRE(cache.get(3) == nullptr);
}


TEST_CASE("Insert Files into Cache") {
   
    lru_cache<2> cache;
   
    REQUIRE(cache.put(1, "testfile1.txt") == true); 
    REQUIRE(cache.put(2, "testfile2.txt") == true);

    REQUIRE(cache.get(1) != nullptr);
    REQUIRE(cache.get(2) != nullptr); 
    REQUIRE(cache.get(3) == nullptr);
    
    REQUIRE(cache.put(3, "testfile3.txt") == true); 
    REQUIRE(cache.get(3) != nullptr);
}


TEST_CASE("Retrieve Files from Cache") {
   
    lru_cache<2> cache;
   
    REQUIRE(cache.put(1, "testfile1.txt") == true);
    void* handle = cache.get(1);
    REQUIRE(handle != nullptr); 
}


TEST_CASE("Handle Retrieval of Non-existent Files") {
   
    lru_cache<2> cache;
    
    REQUIRE(cache.get(999) == nullptr); 
}


TEST_CASE("File Eviction Based on LRU Policy") {
    
    lru_cache<2> cache;
    
    REQUIRE(cache.put(1, "testfile1.txt") == true);
    REQUIRE(cache.put(2, "testfile2.txt") == true);
    REQUIRE(cache.put(3, "testfile3.txt") == true);

    REQUIRE(cache.get(1) == nullptr); 
    REQUIRE(cache.get(2) != nullptr); 
    REQUIRE(cache.get(3) != nullptr); 
}


TEST_CASE("Eviction and Replacement in Single-slot Cache") {
    
    lru_cache<1> cache;
    
    REQUIRE(cache.put(1, "testfile1.txt") == true); 
    REQUIRE(cache.put(2, "testfile2.txt") == true); 

    REQUIRE(cache.get(1) == nullptr); 
    REQUIRE(cache.get(2) != nullptr);
}


TEST_CASE("Attempt to Put Non-existent File") {
    
    lru_cache<2> cache;
    
    REQUIRE_FALSE(cache.put(1, "nonexistent.txt"));
    REQUIRE(cache.get(1) == nullptr);
}


TEST_CASE("Eviction Upon Reaching Max Capacity") {

    lru_cache<3> cache;

    REQUIRE(cache.put(1, "testfile1.txt") == true);
    REQUIRE(cache.put(2, "testfile2.txt") == true);
    REQUIRE(cache.put(3, "testfile3.txt") == true);
    REQUIRE(cache.put(4, "testfile4.txt") == true); 

    REQUIRE(cache.get(1) == nullptr); 
    REQUIRE(cache.get(2) != nullptr); 
    REQUIRE(cache.get(3) != nullptr); 
    REQUIRE(cache.get(4) != nullptr); 
}
