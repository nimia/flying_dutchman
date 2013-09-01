#ifndef GRAPH_H
#define GRAPH_H

#include "sized_ints.h"
#include "list.h"

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
	int32_t max_vertex_num;
	Node nodes[VERTEX__MAX_NUM_OF_VERTICES];
} Graph;

#endif
