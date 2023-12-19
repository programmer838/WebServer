#!/bin/bash

# Compile the C++ programs
g++ -O2 -g -flto -std=c++20 -o atomic_threadpool_perf atomic_threadpool_perf.cpp -lpthread
g++ -O2 -g -flto -std=c++20 -o mutex_threadpool_perf mutex_threadpool_perf.cpp -lpthread

# Function to run perf and extract the desired metrics
run_perf() {
    for ((i = 0; i < 100; i++)); do
        # Run perf and capture the number of CPU cycles, instructions, task-clock, and context-switches
        perf stat -e cycles,instructions,task-clock,context-switches -o perf_output.txt -- ./"$1"
        
        # Extract the metrics from the perf output
        cycles=$(grep "cycles" perf_output.txt | awk '{print $1}' | sed 's/,//g')
        instructions=$(grep "instructions" perf_output.txt | awk '{print $1}' | sed 's/,//g')
        task_clock=$(grep "task-clock" perf_output.txt | awk '{print $1}' | sed 's/,//g')
        context_switches=$(grep "context-switches" perf_output.txt | awk '{print $1}' | sed 's/,//g')
        
        # Calculate instructions per cycle (IPC)
        ipc=$(echo "scale=2; $instructions / $cycles" | bc)
        
        # Write the metrics to the times.txt file
        echo "$1, $cycles, $instructions, $ipc, $task_clock, $context_switches" >> times.txt
    done
}

# Clear the times.txt file
> times.txt

# Add header to times.txt
echo "Type, CPU Cycles, Instructions, IPC, Task Clock (ms), Context Switches" >> times.txt

# Run perf on the atomic_threadpool_perf 100 times
echo "Collecting data for atomic_threadpool_perf..."
run_perf "atomic_threadpool_perf"

# Run perf on the mutex_threadpool_perf 100 times
echo "Collecting data for mutex_threadpool_perf..."
run_perf "mutex_threadpool_perf"

# Remove temporary file
rm perf_output.txt

# Run python script to plot data
python3 plot.py

