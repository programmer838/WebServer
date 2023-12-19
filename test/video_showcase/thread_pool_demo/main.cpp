#include <iostream>
#include <atomic>
#include <thread>
#include "thread_pool.hpp"

void pauseAndExplain(const std::string& message, int time) {

    std::cout <<  message << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(time));
}

void pauseTillEnter() {
    std::cout << "Press Enter to continue..." << '\n';
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    std::atomic<int> taskCounter{0};
    thread_pool<4, 8> pool; // 4 threads, 8 tasks capacity

    std::cout << "+-------------------------------------+\n";
    std::cout << "|Counter variable initial value is: 0 |\n"; 
    std::cout << "+-------------------------------------+\n\n";

    auto incrementTask = [&taskCounter]() { taskCounter.fetch_add(1, std::memory_order_relaxed); };
    pauseAndExplain("Pushing tasks into a specific lock-free SPSC ring buffer.", 3);

    for (int i = 0; i < 4; ++i) {
	 std::string message = "Pushing task " + std::to_string(i+1) + " into ring buffer 0.";
         pauseAndExplain(message, 2);
         pool.push_task(static_cast<uint64_t>(0), incrementTask); // Push to a specific ring buffer
    }

    std::this_thread::sleep_for(std::chrono::seconds(1)); // Allow time for task completion
    std::cout << "Final value of counter after tasks have been executed by thread pool: " << taskCounter.load() << '\n';
    pauseTillEnter();

    pauseAndExplain("\n\nPushing tasks into the general task pool.", 3);
    for (int i = 0; i < 4; ++i) {
	 std::string message = "Pushing task " + std::to_string(i+1) + " into task pool (round-robin task assignment is being used).";
         pauseAndExplain(message, 0.3);
         pool.push_task(incrementTask); // General task submission
    }

    std::this_thread::sleep_for(std::chrono::seconds(1)); // Allow time for task completion
    std::cout << "Task counter after general pool: " << taskCounter.load() << std::endl;
    pauseTillEnter();


    pauseAndExplain("\n\nRapidly pushing 10 tasks in succession.", 3);
    for (int i = 0; i < 10; ++i) { 
	 std::string message = "Pushing task " + std::to_string(i+1) + " into task pool (round-robin task assignment is being used).";
         pauseAndExplain(message, 0);
         pool.push_task(incrementTask); // Rapid task submission
    }
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Allow time for task completion
    std::cout << "Final task counter: " << taskCounter.load() << std::endl;

    pauseAndExplain("Shutting down thread pool.", 3);

    return 0;
}

