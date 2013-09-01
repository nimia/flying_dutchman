all: *.cpp *.h
	g++ -g -std=c++11 -O0 -mcmodel=large -Wall -Wextra -pedantic -Wno-write-strings *.cpp

