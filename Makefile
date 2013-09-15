ALL: test
 
flying_dutchman:
	g++ -g -std=c++11 -O3 -mcmodel=large -Wall -Wextra -pedantic -Wno-write-strings -Wformat dijkstra.cpp main.cpp queue_test.cpp bellman_ford.cpp -o flying_dutchman

test: flying_dutchman
	./flying_dutchman test
	
