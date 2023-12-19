#include <iostream>
#include <fstream>
#include <string.h>
#include <chrono>
#include <thread>
#include "lru_cache.hpp"

void pauseAndExplain(const std::string& message) {
    std::cout << message << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

void pauseTillEnter() {
    std::cout << "Press Enter to continue..." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printCacheState(const lru_cache<3>& cache) {
    std::cout << "+-------------------------------------+" << std::endl;
    std::cout << "| Current State of the Cache          |" << std::endl;
    std::cout << "+-------------------------------------+" << std::endl;

    Node* current = cache.head->next; 

    while (current != cache.tail) { 
        std::cout << "Key: " << current->key << ", File memory address: " << current->file_memory << std::endl;
        current = current->next;
    }
    std::cout << "\n";
}

int main() {

    lru_cache<3> cache;

    // Creating example files
    std::ofstream("file1.txt") << "Content of file 1";
    std::ofstream("file2.txt") << "Content of file 2";
    std::ofstream("file3.txt") << "Content of file 3";

    // Demonstrate 'put' operation for file1
    pauseAndExplain("\nAdding file1.txt to the cache with key 1.");
    cache.put(1, "file1.txt");
    printCacheState(cache);
    pauseTillEnter();

    // Demonstrate 'put' operation for file2
    pauseAndExplain("\nAdding file2.txt to the cache with key 2.");
    cache.put(2, "file2.txt");
    printCacheState(cache);
    pauseTillEnter();

    // Demonstrate 'put' operation for file3
    pauseAndExplain("\nAdding file3.txt to the cache with key 3.");
    cache.put(3, "file3.txt");
    printCacheState(cache);
    pauseTillEnter();

    // Demonstrate 'get' operation for file2
    pauseAndExplain("\nRetrieving file with key 2 from the cache.");
    if (cache.get(2) != nullptr) {
        std::cout << "Retrieved file with key 2." << std::endl;
    }
    printCacheState(cache);
    pauseTillEnter();

    // Demonstrate cache eviction with file4
    pauseAndExplain("\nAdding file4.txt to the cache with key 4, which will trigger eviction of the least recently used item.");
    std::ofstream("file4.txt") << "Content of file 4";
    cache.put(4, "file4.txt");
    printCacheState(cache);

    if (cache.get(1) == nullptr) {
        pauseAndExplain("File with key 1 has been evicted from the cache.");
    }
 
    pauseTillEnter();

    pauseAndExplain("\nThe LRU Cache has eviction capabilities, lets evict the least recently used manually:");
    cache.evict_lru();
    printCacheState(cache);
    pauseTillEnter();    

    return 0;
}

