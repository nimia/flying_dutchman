ALL: test
 
flying_dutchman: *.cpp *.h
	g++ -std=c++11 -Ofast -mcmodel=large -Wall -Wextra -pedantic -Wno-write-strings -Wformat parse.c dijkstra.cpp main.cpp queue_test.cpp bellman_ford.cpp -o flying_dutchman

test: flying_dutchman
	./flying_dutchman test
	
boost: *.cpp *.h
	g++ -Ofast boost_test.cpp parse.c -o boost