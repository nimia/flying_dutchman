#ifndef GRAPH_H
#define GRAPH_H

#include "sized_ints.h"
#include "list.h"

#include <list>
using namespace std;

typedef struct Vertex Vertex;

typedef struct Edge {
	Vertex *dest;
	Distance distance;
} Edge;

typedef struct Vertex {
	struct list_head equi_distance_vertices;
	list<Edge> edges;
	Distance distance;
	Vertex_Num vertex_num;
} Vertex;

typedef struct Graph {
	int32_t max_vertex_num;
	Vertex vertices[VERTEX__MAX_NUM_OF_VERTICES];
} Graph;

#endif
