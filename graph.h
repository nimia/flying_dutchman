#ifndef GRAPH_H
#define GRAPH_H

#include "sized_ints.h"
#include "list.h"

#include <list>
using namespace std;

typedef struct Vertex Vertex;
#include "queue__per_vertex_data.h"

typedef struct Edge {
	Vertex *dest;
	Distance distance;
} Edge;

typedef struct Vertex {
	Vertex_Num vertex_num;
	list<Edge> edges;
	Distance distance;

	Queue__Per_Vertex_Data queue_data;
} Vertex;

typedef struct Graph {
	int32_t max_vertex_num;
	Vertex vertices[VERTEX__MAX_NUM_OF_VERTICES];
} Graph;

#endif
