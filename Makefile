all: dijkstra.cpp list.h huge_header.h queue.h
	g++ -std=c++11 -O0 -Wall -Wextra -fpermissive dijkstra.cpp

