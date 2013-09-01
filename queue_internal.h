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
 * nodes with that distance.
 * For space reduction reasons, it doesn't hold a list_head per-se for each possible distance,
 * but a Vertex_Num. If the "conceptual" linked-list is empty, the Vertex_Num is INVALID_VERTEX.
 * If it isn't empty, the Vertex_Num is a number of any of the nodes which currently have said distance,
 * which are all linked in a circular linked-list courtesy of list.h from the kernel (with minor modifications).
 *
 * Inserting and deleting a node are trivial.
 * pop_min() employs a trick: since we're using it only for Dijkstra, once we did a pop_min(),
 * we are guaranteed that subsequent pop_min's can only return vertex with greater distance than the result.
 * So basically, we need to maintain a running index for the array of linked lists (whose size is the
 * number of possible distances), and on pop_min() either pop an element from the current linked list,
 * or, if it's empty, increment the index and try again.
 */

typedef struct Queue {
	Vertex_Num equi_distance_nodes_head_vertex_num[DISTANCE_NUM_OF_POSSIBLE_VALUES];
	int64_t min_distance_candidate;

	// this does NOT decrement on decrease_key for code simplicity purposes; it's really "max ever seen"
	// it isn't of type Distance to avoid any over/under flow
	int64_t max_distance_ever_seen;
} Queue;

static inline void Queue__init(Queue *queue)
{
	for (int64_t i = 0; i < DISTANCE_NUM_OF_POSSIBLE_VALUES; i++) {
		queue->equi_distance_nodes_head_vertex_num[i] = INVALID_VERTEX;
	}
	queue->min_distance_candidate = 0;
	queue->max_distance_ever_seen = 0;
}

static inline void Queue__insert(Queue *queue, Node *node, Distance distance, struct Graph *graph)
{
	DEBUG("inserting %d with distance %d\n", node->vertex_num, distance);

	if (distance >= DISTANCE_MAX) {
		printf("Encountered too great of a distance: %d, for node %d\n", distance, node->vertex_num);
		abort();
	}

	node->distance = distance;
	Vertex_Num *equi_distance_nodes_head_vertex_nump = &queue->equi_distance_nodes_head_vertex_num[distance];
	Vertex_Num equi_distance_nodes_head_vertex_num = *equi_distance_nodes_head_vertex_nump;

	if (equi_distance_nodes_head_vertex_num != INVALID_VERTEX) {
		list_add(&node->equi_distance_nodes, &graph->nodes[equi_distance_nodes_head_vertex_num].equi_distance_nodes);
	} else {
		INIT_LIST_HEAD(&node->equi_distance_nodes);
		*equi_distance_nodes_head_vertex_nump = node->vertex_num;
	}

	queue->max_distance_ever_seen = MAX(queue->max_distance_ever_seen, distance);
}

static inline void Queue__delete(Queue *queue, Node *node)
{
	Vertex_Num *equi_distance_nodes_head_vertex_nump = &queue->equi_distance_nodes_head_vertex_num[node->distance];

	if (list_empty(&node->equi_distance_nodes)) {
		*equi_distance_nodes_head_vertex_nump = INVALID_VERTEX;
	} else {
		Node *new_head = list_entry(node->equi_distance_nodes.next, Node, equi_distance_nodes);
		*equi_distance_nodes_head_vertex_nump = new_head->vertex_num;
		list_del_init(&node->equi_distance_nodes);
	}
}

static inline Node *Queue__pop_min(Queue *queue, Graph *graph)
{
	while (queue->min_distance_candidate <= queue->max_distance_ever_seen) {
		Vertex_Num *equi_distance_nodes_head_vertex_nump = &queue->equi_distance_nodes_head_vertex_num[queue->min_distance_candidate];
		Vertex_Num equi_distance_nodes_head_vertex_num = *equi_distance_nodes_head_vertex_nump;

		if (equi_distance_nodes_head_vertex_num != INVALID_VERTEX) {
			Node *node = &graph->nodes[equi_distance_nodes_head_vertex_num];
			Queue__delete(queue, node);
			DEBUG("popped min %d with distance %d\n", node->vertex_num, node->distance);
			return node;
		}
		queue->min_distance_candidate++;
	}

	return NULL;
}

static inline void Queue__decrease_key(Queue *queue, Node *node, Distance new_distance, Graph *graph)
{
	Queue__delete(queue, node);

	Queue__insert(queue, node, new_distance, graph);
}

#endif
