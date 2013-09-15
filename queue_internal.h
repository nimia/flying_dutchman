#ifndef QUEUE_INTERNAL_H_
#define QUEUE_INTERNAL_H_

#include <stdlib.h>

#include "list.h"
#include "graph.h"
#include "utils.h"

/*
 * TLDR: This queue is intended for use only for the Dijkstra algorithm - it WILL provide erroneous results
 * if used under different assumptions. For details, see below.
 *
 * This queue works quite differently from comparison-based queues, so some documentation is in order:
 * Conceptually, since distances are ints, for each possible distance, the queue holds a linked list of
 * vertices with that distance.
 * For space reduction reasons, it doesn't hold a list_head per-se for each possible distance,
 * but a Vertex_Num. If the "conceptual" linked-list is empty, the Vertex_Num is INVALID_VERTEX.
 * If it isn't empty, the Vertex_Num is a number of any of the vertices which currently have said distance,
 * which are all linked in a circular linked-list courtesy of list.h from the kernel (with minor modifications).
 *
 * Inserting and deleting a vertex are trivial.
 * pop_min() employs a trick: since we're using it only for Dijkstra, once we did a pop_min(),
 * we are guaranteed that subsequent pop_min's can only return vertex with greater distance than the result.
 * So basically, we need to maintain a running index for the array of linked lists (whose size is the
 * number of possible distances), and on pop_min() either pop an element from the current linked list,
 * or, if it's empty, increment the index and try again.
 */

typedef struct Queue {
	Vertex_Num equi_distance_vertices_head_vertex_num[DISTANCE__NUM_OF_POSSIBLE_VALUES];
	int64_t min_distance_candidate;

	// this does NOT decrement on decrease_key for code simplicity purposes; it's really "max ever seen"
	// it isn't of type Distance to avoid any over/under flow
	int64_t max_distance_ever_seen;
} Queue;

static inline void Queue__init(Queue *queue)
{
	for (int64_t i = 0; i < DISTANCE__QUEUE_QUICK_INIT_PREFIX_LENGTH; i++) {
		queue->equi_distance_vertices_head_vertex_num[i] = VERTEX__INVALID_VERTEX;
	}
	queue->min_distance_candidate = 0;
	queue->max_distance_ever_seen = 0;
}

static inline void Queue__insert(Queue *queue, Vertex *vertex, Distance distance, struct Graph *graph)
{
	DEBUG("inserting %d with distance %d\n", vertex->vertex_num, distance);

	if (queue->max_distance_ever_seen < distance) {
		for (uint64_t i = queue->max_distance_ever_seen + 1; i <= distance; i++) {
			queue->equi_distance_vertices_head_vertex_num[i] = VERTEX__INVALID_VERTEX;
		}

		queue->max_distance_ever_seen = distance;
	}

	vertex->distance = distance;
	Vertex_Num *equi_distance_vertices_head_vertex_nump = &queue->equi_distance_vertices_head_vertex_num[distance];
	Vertex_Num equi_distance_vertices_head_vertex_num = *equi_distance_vertices_head_vertex_nump;

	if (equi_distance_vertices_head_vertex_num != VERTEX__INVALID_VERTEX) {
		list_add(&vertex->equi_distance_vertices, &graph->vertices[equi_distance_vertices_head_vertex_num].equi_distance_vertices);
	} else {
		INIT_LIST_HEAD(&vertex->equi_distance_vertices);
		*equi_distance_vertices_head_vertex_nump = vertex->vertex_num;
	}
}

static inline void Queue__delete(Queue *queue, Vertex *vertex)
{
	Vertex_Num *equi_distance_vertices_head_vertex_nump = &queue->equi_distance_vertices_head_vertex_num[vertex->distance];

	if (list_empty(&vertex->equi_distance_vertices)) {
		*equi_distance_vertices_head_vertex_nump = VERTEX__INVALID_VERTEX;
	} else {
		Vertex *new_head = list_entry(vertex->equi_distance_vertices.next, Vertex, equi_distance_vertices);
		*equi_distance_vertices_head_vertex_nump = new_head->vertex_num;
		list_del_init(&vertex->equi_distance_vertices);
	}
}

static inline Vertex *Queue__pop_min(Queue *queue, Graph *graph)
{
	while (queue->min_distance_candidate <= queue->max_distance_ever_seen) {
		Vertex_Num *equi_distance_vertices_head_vertex_nump = &queue->equi_distance_vertices_head_vertex_num[queue->min_distance_candidate];
		Vertex_Num equi_distance_vertices_head_vertex_num = *equi_distance_vertices_head_vertex_nump;

		if (equi_distance_vertices_head_vertex_num != VERTEX__INVALID_VERTEX) {
			Vertex *vertex = &graph->vertices[equi_distance_vertices_head_vertex_num];
			Queue__delete(queue, vertex);
			DEBUG("popped min %d with distance %d\n", vertex->vertex_num, vertex->distance);
			return vertex;
		}
		queue->min_distance_candidate++;
	}

	return NULL;
}

static inline void Queue__decrease_key(Queue *queue, Vertex *vertex, Distance new_distance, Graph *graph)
{
	Queue__delete(queue, vertex);

	Queue__insert(queue, vertex, new_distance, graph);
}

#endif
