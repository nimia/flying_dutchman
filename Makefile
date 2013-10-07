ALL: test
 
flying_dutchman: *.c *.h
	gcc -I./monotonic_queue -std=c11 -Ofast -mcmodel=large -Wall -Wextra -pedantic -Wno-write-strings -Wformat parse.c \
	 dijkstra.c main.c queue_test.c bellman_ford.c -o flying_dutchman

test: flying_dutchman
	./flying_dutchman test
	
boost: *.c *.cpp *.h
	g++ -Ofast boost_test.cpp parse.c -o boost
	
clean:
	rm boost flying_dutchman bellman_ford