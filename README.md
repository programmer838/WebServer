CS3821 — BSc Final Year Project 
===============================================
Design and Implementation of a Multithreaded Web Server
===============================================

This repository contains the source code, tests, and documentation for my BSc final year project, focused on the design and implementation of a multithreaded web server aimed at achieving high performance and low latency.

Repository Structure
---------------------------
- ```include/``` - Various C/C++ header files (```.hpp```/```.h```) which make up the core components of the project.
    - ```lru_cache/``` - Contains the implementation of the LRU cache component of the project.
    - ```thread_pool/``` - Contains implementation of the thread pool component of the project.
- ```src/``` - Contains ```.cpp``` files which make up the core web server.
    - ```single_threaded_server/``` - Contains implementation of the single threaded web server
- ```test/``` - Contains various testing and video showcase files and scripts.
    - ```lru_cache_tests/``` - Contains LRU Cache unit testing files
    - ```thread_pool_tests/``` Contains thread pool testing files
    - ```thread_pool_performance_test/``` - Thread pool performance testing files.
    - ```video_showcase/``` - Files and scripts shown in Interim video deliverable.

Term 1 Early Deliverables
=============================

Thread pool
-----------
The thread pool is a key component designed to manage and optimize concurrent task execution in the web server. It efficiently handles multiple client requests by distributing them across a set number of worker threads. This structure not only improves the server's performance by reducing the overhead of constant thread creation and destruction but also ensures balanced task allocation and improved resource utilization. The thread pool serves as the backbone for the server's ability to handle high concurrency, it will serve as a core component in the multi-threaded architecture of the web server.

LRU Cache
---------
The Least Recently Used (LRU) Cache, integrated with memory mapping, is a cache developed to manage frequently accessed data. It significantly reduces disk I/O by storing and retrieving the most commonly requested files directly from memory. This cache mechanism employs a policy that evicts the least recently accessed items, ensuring that the cache always contains the most relevant data. The integration of memory mapping further enhances performance by allowing direct file access from memory, greatly accelerating data retrieval times and reducing server response latency.

Single-threaded Web Server
---------------------------
The single-threaded web server forms the initial phase of the project, providing a fundamental understanding of web server operations. It handles HTTP requests sequentially, processing one request at a time. While this approach limits the server's ability to handle multiple requests simultaneously, it lays the groundwork for understanding basic server functionalities such as request handling, URL decoding, and file serving. The insights gained from this single-threaded server are vital in developing the subsequent multi-threaded version, highlighting areas for improvement and scalability.


Usage/Installation
==================

In order to use the files in this repository, please follow the following instructions:

1) Clone the git repository, you can use the following command to do so
```
git clone https://gitlab.cim.rhul.ac.uk/zjac319/PROJECT.git
```
You will then be prompted to give a username and password, which you need to provide
```
Cloning into 'PROJECT'...
Username for 'https://gitlab.cim.rhul.ac.uk': 
Password for 'https://gitlab.cim.rhul.ac.uk': 
```

2) After cloning the repository change you current working directory to the ```PROJECT``` directory:
```
cd PROJECT
```

Usage
=====

After cloning the repository users can access all files, tests and programs in the clone:

Using the thread pool 
----------------------

There are three main ways to interact with the thread pool directly from the repository without intergating it into your own project:

1) **Run the thread pool Catch2 tests to verify if the functionality works as intended.**
    - Navigate to the ```test/thread_pool_tests``` directory with following command: 
    ```
    cd test/thread_pool_tests/
    ```
    - Compile and run the ```ring_buffer_test.cpp``` or ```thread_pool_test.cpp``` tests, users should use the ```g++``` compiler. Please make sure you have [Catch2](https://github.com/catchorg/Catch2) installed on your system.
    ```
    g++ FILE_NAME_test.cpp -flto -O3 -march=native -lpthread -std=c++20    ```
    ```
    - Run the executable to view test cases pass:
    ```
    ./a.out
    ```
2) **Run the thread pool video showcase scripts to get a live view of it in action.**

    - Navigate to the ```test/video_showcase/thread_pool_demo``` directory with the following command:
    ```
    cd test/video_showcase/thread_pool_demo
    ```
    - Ensure that the file ```thread_pool_demo.sh``` has valid permissions:
    ``` 
    sudo chmod 500 thread_pool_demo.sh
    ```
    - Execute the demo:
    ```
    ./thread_pool_demo.sh
    ```
    
3) **Run the thread pool performance tests.**

- Navigate to ```test/thread_pool_tests/thread_pool_performance_test``` directory:
  ```
  cd test/thread_pool_tests/thread_pool_performance_test/
  ```
- Ensure the ```run_performance_test.sh``` bash script has valid permissions:
  ```
   sudo chmod 500 run_performance_test.sh
  ``` 
- Ensure you have the necessary permissions to access CPU performance counters on Linux:
    ``` 
    echo 1 | sudo tee /proc/sys/kernel/perf_event_paranoid
    ```
- Execute performance tests:
  ```
  ./run_performance_test.sh
  ```
- Set CPU performance counter permissions back to default:
  ```
  echo -1 | sudo tee /proc/sys/kernel/perf_event_paranoid
  ```
  
Using the LRU Cache
----------------------

There are two main ways to interact with the LRU Cache directly from the repository:

1) **Run the LRU Cache Catch2 tests.**

- Navigate to the ```test/lru_cache_tests``` directory:
    ```
    cd test/lru_cache_tests/
    ```
-  Compile the LRU Cache test file:
    ```
    g++ lru_cache_test.cpp -flto -O3 -march=native -lpthread -std=c++20    ```
    ```
- Execute the test file:
    ```
    ./a.out
    ```

2) **Run the LRU Cache video showcase scripts to get a live view of the LRU Cache in action.**

- Navigate to the ```test/video_showcase/lru_cache_demo``` directory:
    ```
    cd test/video_showcase/lru_cache_demo
    ```
- Ensure the ```lru_demo.sh``` file has valid permissions:
    ```
    sudo chmod 500 lru_demo.sh
    ```
- Execute the demo script:
    ```
    ./lru_demo.sh
    ```
Using the Single-threaded Web Server
------------------------------------

The single-threaded web server component can be used and accessed as follows:

- Navigate to the ```src/single_threaded_server/``` directory
    ```
    cd src/single_threaded_server/
    ```
- Compile the server:
    ```
    g++ single_threaded_server.cpp -flto -O3 -march=native -lpthread -std=c++20
    ```
- Run the server:
    ```
    ./a.out
    ```
- Navigate to ```http://127.0.0.1:8080/``` on a web browser such as Firefox or Google Chrome


Interim Video Deliverable
==========================

The YouTube link for the unlisted term 1 deliverable showcase video is: https://www.youtube.com/watch?v=VsUibYaEjcs

Authors
=============
- Qasim Ijaz
