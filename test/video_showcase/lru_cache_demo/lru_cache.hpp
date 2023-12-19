#pragma once

#include <iostream>
#include <memory>
#include <cstdint>
#include <mutex>
#include <unordered_map>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

struct Node {
  
    int key;
    void *file_memory;
    size_t file_size;

    Node *next;
    Node *prev;

    Node(int key, size_t file_size, Node *prev, Node *next)
        : key(key), file_memory(nullptr), file_size(file_size), prev(prev), next(next) {}

    Node() 
        : key(0), file_memory(nullptr), file_size(0), next(nullptr), prev(nullptr) {}
};

template <uint64_t capacity>
class lru_cache {

public:
    lru_cache() noexcept : head{new Node()}, tail{new Node()}, count{0} {

	head->next = tail; 
        tail->prev = head;
    }

    lru_cache(const lru_cache& p) = delete;
    lru_cache(lru_cache&& p) = delete;
    lru_cache& operator=(const lru_cache&) = delete;
    lru_cache& operator=(const lru_cache&&) = delete;

    void *get(int key) noexcept {
               
         const std::lock_guard<std::mutex> lock(cache_mutex);

         if (map.find(key) != map.end()) {
             Node *node = map[key];
             move_to_start(node);
             return node->file_memory;
         }
 
         return nullptr;
    }

    bool put(int key, const char *filename) {
	
         const std::lock_guard<std::mutex> lock(cache_mutex);
 
         int fd = open(filename, O_RDONLY);

         if (fd == -1) { 
             std::cerr << "Failed to open file " + std::string(filename);
             return false;
         }

         struct stat sb;

         if (fstat(fd, &sb) == -1) {
             std::cerr << "Failed to obtain stats for file " + std::string(filename);
             close(fd);
             return false;
         }

         void *file_memory = mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
              
         if (file_memory == MAP_FAILED) {
             std::cerr << "Failed to map file " + std::string(filename);
             close(fd); 
             return false;
         }

         close(fd);

         if (map.find(key) == map.end()) {
            
             if (count == capacity) { 
	         evict_lru();
	     }

             Node *node = new Node(key, sb.st_size, head, head->next);
             node->file_memory = file_memory;
            
             map[key] = node;
             count++;
            
             head->next->prev = node;
             head->next = node;
         }
         else {

             Node *node = map[key];

             if (munmap(node->file_memory, node->file_size) == -1) {
    		 int error = errno;
  	         std::cerr << "Error unmapping memory for file " + std::string(filename) + ": " << strerror(error) << '\n';
		 return false;
	     }

             node->file_memory = file_memory;
             node->file_size = sb.st_size;

             move_to_start(node);
         }

	 return true; 
    }

    ~lru_cache() {

         Node* current = head->next;
        
         while (current != tail) {
 
              Node* to_delete = current;
              current = current->next;
 
              munmap(to_delete->file_memory, to_delete->file_size);
              delete to_delete;
         }

         delete head;
         delete tail;
    }

    std::unordered_map<int, Node *> map;
    std::mutex cache_mutex;

    Node *head;
    Node *tail;
   
    uint64_t count;

    void move_to_start(Node *node) noexcept {
   
         node->prev->next = node->next;
         node->next->prev = node->prev;

         node->next = head->next;
         node->next->prev = node;
         node->prev = head;
         head->next = node;
    }

    void evict_lru() {

         Node *node = tail->prev;

         if (munmap(node->file_memory, node->file_size) == -1) {
             int error = errno;
             std::cerr << "Error unmapping file memory during LRU eviction: " << strerror(error) << '\n';
         }

         node->next->prev = node->prev; 
         node->prev->next = node->next;

         map.erase(node->key);
         count--;
 
         delete node;
    }
};
