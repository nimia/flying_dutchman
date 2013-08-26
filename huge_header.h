#ifndef GLOBAL_H
#define GLOBAL_H

#include "sized_ints.h"

#include <list>
using namespace std;

typedef struct Node Node;

typedef struct Node_And_Distance {
	Node *node;
	Distance distance;
} Node_And_Distance;

typedef struct Node {
	struct list_head equi_distance_nodes;
	list<Node_And_Distance> neighbors;
	Distance distance;
	Vertex_Num vertex_num;
} Node;

typedef struct Graph {
	Node nodes[NUM_OF_VERTICES];
} Graph;

#endif
