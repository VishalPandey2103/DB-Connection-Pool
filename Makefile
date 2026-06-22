CXX = g++
CXXFLAGS = -Wall -pthread

all: pool

pool: main.cpp connection.cpp connection_pool.cpp
	$(CXX) $(CXXFLAGS) -o pool main.cpp connection.cpp connection_pool.cpp

clean:
	rm -f pool