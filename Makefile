all: *.cpp *.h
	g++ -g -std=c++11 -O3 -Wall -Wextra -pedantic -Wno-write-strings *.cpp

