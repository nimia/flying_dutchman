all: *.cpp *.h
	g++ -g -std=c++11 -O3 -mcmodel=large -Wall -Wextra -pedantic -Wno-write-strings -Wformat dijkstra.cpp main.cpp queue_test.cpp


