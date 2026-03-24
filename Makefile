CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread -Iinclude
TARGET   = bin/mc-sentinel

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:src/%.cpp=bin/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f bin/*.o $(TARGET)
