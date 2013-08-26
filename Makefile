all: *.cpp *.h
	g++ -g -std=c++11 -O0 -Wall -Wextra -Wno-write-strings dijkstra.cpp

