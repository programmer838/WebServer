# Compiler settings
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Werror
LDFLAGS = -luring

# Source files and target executable name
SRCS = main.cpp
TARGET = my_program

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)

