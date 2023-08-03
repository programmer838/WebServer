#include <iostream>
#include <liburing.h>

int main(int argc, char **argv)
{

    
    return 0;
}

/*

Aims:

    Emphasize Network Programming: Implement basic client-server communication using TCP/IP in C++.
    Focus on Multithreading: Explore multithreading techniques and design a server that handles multiple client connections simultaneously.

Background:

    Networking Basics: Work with TCP/IP protocols to handle client-server communication.
    Multithreading Concepts: Understand and implement threading models to manage multiple client connections efficiently.

Early Deliverables:

    Basic Network Server: A single-threaded server that can handle one client at a time using a simple custom protocol over TCP/IP.
    Network Programming Report: Detailing the network architecture, protocols used, and C++ implementation specifics.

Advanced Features:

    Multithreaded Architecture: Upgrade the server to a multithreaded model using C++ threading libraries, allowing simultaneous handling of multiple clients.
    Simple Request-Response Protocol: Implement a minimal protocol to receive and respond to basic client requests (e.g., sending a welcome message or echoing client input).
    Performance Optimization: Apply C++ techniques to reduce latency and improve server performance, focusing on efficient thread management.

Final Deliverables:

    Fully Operational Multithreaded Network Server: Capable of efficiently handling multiple simultaneous client connections, adhering to a minimal protocol for simple interaction.
    Comprehensive Documentation: Including the network programming techniques, C++ threading implementation, and clear instructions for setup, use, and troubleshooting.
    Performance Analysis Reports: Assessing the server's efficiency, latency, and scalability.

Relevance to Network Programming and HFT:

    Networking Mastery: In-depth work with TCP/IP and custom protocol provides hands-on experience with core network programming concepts in C++.
    Multithreading Skills: The development of a multithreaded server architecture aligns with the concurrent processing requirements of HFT systems.

Technology Stack:

    Programming Language: C++, capitalizing on modern C++ features and libraries for network and threading tasks.
    Networking Libraries: Utilize suitable C++ networking libraries like Boost.Asio or standard networking facilities in C++17 or later.
*/