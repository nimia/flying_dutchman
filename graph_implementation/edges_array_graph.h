#ifndef EDGES_ARRAY_GRAPH_H_
#define EDGES_ARRAY_GRAPH_H_

#include <stdlib.h> // NULL
#include <stdio.h> // printf (for too many edges)

#include "../utils.h"
#include "../sized_ints.h"
#include "../list.h"

#define MIN_EDGES_CAPACITY 16

typedef struct Vertex Vertex;
#include "queue__per_vertex_data.h"

typedef struct Edge {
	Vertex *dest;
	Distance distance;
} Edge;

typedef struct Vertex {
	Vertex_Num vertex_num;
	Distance distance;

	Edge *edges;
	uint32_t num_of_edges;
	uint32_t edges_capacity;

	Queue__Per_Vertex_Data queue_data;
} Vertex;

#define Vertex__FOR_EACH_EDGE(vertex, edge_iterator) \
	for (edge_iterator = &(vertex)->edges[0]; edge_iterator < &(vertex)->edges[(vertex)->num_of_edges]; \
		 edge_iterator++)

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
	if (v->edges_capacity <= v->num_of_edges) {
		uint32_t new_capacity = MAX(v->edges_capacity * 2, MIN_EDGES_CAPACITY);
		v->edges = (Edge *)realloc(v->edges, new_capacity * sizeof(Edge));
		v->edges_capacity = new_capacity;
	}

	v->edges[v->num_of_edges] = *e;
	v->num_of_edges++;
}

static inline void Graph__reset(Graph *graph)
{
	for (int i = 0; i < GRAPH__MAX_NUM_OF_VERTICES; i++) {
		Vertex *vertex = &graph->vertices[i];

		vertex->vertex_num = i;
		vertex->distance = DISTANCE__INFINITY;

		vertex->num_of_edges = 0;
		vertex->edges = NULL;
		vertex->edges_capacity = 0;

		Queue__Per_Vertex_data__init(&vertex->queue_data);
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
