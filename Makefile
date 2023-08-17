# Compiler settings
CXX = clang++
CXXFLAGS = -std=c++20 -stdlib=libc++ -Wall -Werror
LDFLAGS = -luring -lpthread

# Source files and target executable name
SRCS = main.cpp
TARGET = my_program

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
