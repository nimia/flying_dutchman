all: dijkstra.cpp list.h huge_header.h queue.h
	g++ -g -std=c++11 -O0 -Wall -Wextra -Wno-write-strings dijkstra.cpp

