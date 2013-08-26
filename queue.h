#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"
#include "huge_header.h"
#include "utils.h"

typedef struct Queue {
	Vertex_Num nodes_with_distance_head[DISTANCE_NUM_OF_POSSIBLE_VALUES];
	int64_t current_cell_num;

	// this does NOT decrement on decrease_key for code simplicity purposes;
	// it isn't of type Distance to avoid any over/under flow
	int64_t max_distance_ever_seen;
} Queue;

static inline void Queue__init(Queue *queue)
{
	for (uint64_t i = 0; i < DISTANCE_NUM_OF_POSSIBLE_VALUES; i++) {
		queue->nodes_with_distance_head[i] = INVALID_VERTEX;
	}
	queue->current_cell_num = 0;
	queue->max_distance_ever_seen = 0;
};

static inline void Queue__insert(Queue *queue, Node *node, Distance distance, struct Graph *graph)
{
	DEBUG("inserting %d with distance %d\n", node->vertex_num, distance);
	node->distance = distance;
	Vertex_Num *cell = &queue->nodes_with_distance_head[distance];

	if (*cell != INVALID_VERTEX) {
		list_add(&node->equi_distance_nodes, &graph->nodes[*cell].equi_distance_nodes);
	} else {
		INIT_LIST_HEAD(&node->equi_distance_nodes);
		*cell = node->vertex_num;
	}

	queue->max_distance_ever_seen = MAX(queue->max_distance_ever_seen, distance);
};

// this should be internal
static inline void Queue__delete(Queue *queue, Node *node)
{
	Vertex_Num *cell = &queue->nodes_with_distance_head[node->distance];

	if (list_empty(&node->equi_distance_nodes)) {
		*cell = INVALID_VERTEX;
	} else {
		*cell = list_entry(node->equi_distance_nodes.next, Node, equi_distance_nodes);
		list_del_init(&node->equi_distance_nodes);
	}
}

static inline Node *Queue__pop_min(Queue *queue, Graph *graph)
{
	while (queue->current_cell_num <= queue->max_distance_ever_seen) {
		Vertex_Num *current_cell = &queue->nodes_with_distance_head[queue->current_cell_num];
		Vertex_Num vertex_num = *current_cell;

		if (vertex_num != INVALID_VERTEX) {
			Node *node = &graph->nodes[vertex_num];
			Queue__delete(queue, node);
			DEBUG("popped min %d\n", node->vertex_num);
			return node;
		}
		queue->current_cell_num++;
	}

	return NULL;
};

static inline void Queue__decrease_key(Queue *queue, Node *node, Distance new_distance, Graph *graph)
{
	Queue__delete(queue, node);

	Queue__insert(queue, node, new_distance, graph);
}

#endif
