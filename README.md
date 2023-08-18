# Project Title: Design and Implementation of a Multithreaded Web Server

## Aims

The central objective of this project is to design, build, and evaluate a multithreaded web server capable of handling multiple client requests simultaneously with efficiency and robustness. This project aims to impart a profound understanding of server architecture, multithreading concepts, HTTP protocols, and network programming. Furthermore, it seeks to translate these theoretical concepts into practical skills, resulting in the development of a scalable network application capable of managing high loads without sacrificing performance or stability.

## Background 

Web servers constitute the bedrock of the internet, delivering content to client applications such as web browsers. With the surge in demand for web-based services, these servers must efficiently manage a multitude of simultaneous requests. One such solution is a multithreaded web server, which processes multiple requests concurrently by assigning each request to a separate thread. This project aims to explore this concept in depth, beginning with the fundamentals of web servers, understanding the intricacies of HTTP communication, and gradually building up to the complex subject of multithreading, thread safety, and synchronization.

 

## Early Deliverables

- A basic, single-threaded web server that can handle one client request at a time, showcasing a grasp of the client-server model, HTTP protocol, and network programming. 

- A report showing a strong understanding and overview of the HTTP protocol, including the request/response model, methods, status codes, headers, and the underlying TCP/IP model. 

- A comprehensive design document illustrating the architecture and workflow of the multithreaded server. This should include identifying potential challenges in multithreading, such as race conditions or deadlocks, and proposing strategies for handling these issues. 

- Thread Pool PoC: This would demonstrate how to create a pool of threads that can be used to perform tasks. It might, for example, accept a number of tasks and a number of threads as input, then distribute those tasks among the threads in the pool. 

- Memory-Mapped Files for Efficient I/O Operations PoC: The objective of this PoC is to showcase the efficient usage of memory-mapped files in a custom web server, improving performance by eliminating explicit read/write operations and reducing I/O latency. 

- LRU Cache PoC: This would demonstrate a caching mechanism that evicts the least-recently-used item when the cache is full, and a new item needs to be added.  

- A report on optimization strategies for low latency and high performance in web servers which will delve into the various techniques and principles employed to enhance the performance of a web server.

 

## Final Deliverables

- A fully operational multithreaded web server proficient in efficiently handling multiple simultaneous client requests without thread contention or excessive thread creation and destruction. 

- Comprehensive documentation including server design, key features, explanation behind implementation choices, potential bottlenecks, and clear instructions for setup, use and troubleshooting. 

- Rigorous testing and performance analysis reports, detailing the server's behaviour under different loads, error conditions, and comparison with a single-threaded server. These reports should include measures of response time, throughput, and resource usage, among other metrics.




# Thread pool notes

- Avoid Allocation and Deallocation of objects, this can be quite costly
- Use fixed size objects instead of dynamic ones, this avoids memory allocation and resizing overhead
- Reuse memory, create memory pool so memory can be reused and recycled
- Use move semantics over copying data
- 

# Rules of performance 
- Never guess about performance
- Only measure what is relevant
