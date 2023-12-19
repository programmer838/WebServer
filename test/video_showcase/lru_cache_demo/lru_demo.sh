#!/bin/bash

echo "Compiling the LRU cache demonstration program..."
g++ -o lru_demo main.cpp -lpthread -std=c++17

if [ ! -f lru_demo ]; then
    echo "Compilation failed. Exiting the script."
    exit 1
fi

echo "Compilation successful. Starting the demonstration."
echo "Please follow the on-screen messages for a guided tour of the LRU cache functionality."

# Run the compiled program
./lru_demo

# Clean up after the demonstration
echo "Cleaning up created files and demonstration artifacts..."
rm lru_demo file1.txt file2.txt file3.txt file4.txt

echo "Demonstration concluded. Thank you for your attention."

