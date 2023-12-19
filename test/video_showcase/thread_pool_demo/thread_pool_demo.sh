#!/bin/bash

# Compile the program
g++ -o thread_pool_demo main.cpp -lpthread -std=c++20

# Check if compilation succeeded
if [ ! -f thread_pool_demo ]; then
    echo "Compilation failed. Exiting the script."
    exit 1
fi

echo -e "Compilation successful.\n\nRunning the demonstration...\n\n"

# Run the compiled program
./thread_pool_demo

# Clean up the compiled binary after the demonstration
echo -e "\nDemonstration complete.\n\nCleaning up...\n\n"
rm thread_pool_demo

# End of the script
echo "Thank you for watching the demonstration of the thread pool."

