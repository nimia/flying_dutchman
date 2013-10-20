#ifndef GRAPH_IMPLEMENTATION_H_
#define GRAPH_IMPLEMENTATION_H_

#include <stdlib.h> // NULL
#include <stdio.h> // printf (for too many edges)

#include "../sized_ints.h"
#include "../list.h"

typedef struct Vertex Vertex;
#include "queue__per_vertex_data.h"

typedef struct Edge {
	Vertex *dest;
	Distance distance;
	struct list_head list;
} Edge;

typedef struct Vertex {
	Vertex_Num vertex_num;
	Distance distance;
	struct list_head edges_list;

	Queue__Per_Vertex_Data queue_data;
} Vertex;

#define GRAPH__MAX_NUM_OF_VERTICES 23947348
#define GRAPH__MAX_NUM_OF_EDGES 60000000

typedef struct Graph {
	int32_t max_vertex_num;
	int32_t num_of_edges;
	Vertex vertices[GRAPH__MAX_NUM_OF_VERTICES];
	Edge edges[GRAPH__MAX_NUM_OF_EDGES];
} Graph;

static inline void Vertex__add_edge(Vertex *v, Edge *e)
{
	list_add_tail(&e->list, &v->edges_list);
}

static inline void Graph__reset(Graph *graph)
{
	for (int i = 0; i < GRAPH__MAX_NUM_OF_VERTICES; i++) {
		Vertex *vertex = &graph->vertices[i];
		INIT_LIST_HEAD(&vertex->queue_data.equi_distance_vertices);
		INIT_LIST_HEAD(&vertex->edges_list);
		vertex->distance = DISTANCE__INFINITY;
		vertex->vertex_num = i;
	}

	graph->num_of_edges = 0;
	graph->max_vertex_num = -1;
}

static inline void Graph__add_edge(Graph *graph, Vertex *from, Vertex *to, Distance distance)
{
	if (graph->num_of_edges == GRAPH__MAX_NUM_OF_EDGES) {
		printf("Too many edges - did you remember to set GRAPH__MAX_NUM_OF_EDGES?\n");
		abort();
	}

	Edge *e = &graph->edges[graph->num_of_edges];
	e->dest = to;
	e->distance = distance;
	graph->num_of_edges++;
	Vertex__add_edge(from, e);
}

#endif
