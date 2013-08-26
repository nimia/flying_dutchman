#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"
#include "huge_header.h"

typedef struct Queue {
	Vertex_Num nodes_with_distance_head[DISTANCE_NUM_OF_POSSIBLE_VALUES];
	int64_t current_cell_num;
	int64_t max_distance_ever_seen; // this does NOT decrement on decrease_key for code simplicity purposes;
									// this isn't of type Distance to avoid any over/under flow
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
	ASSERT(list_empty(&node->equi_distance_nodes));

	if (queue->nodes_with_distance_head[distance] != INVALID_VERTEX) {
		list_add(&node->equi_distance_nodes, &graph->nodes[queue->nodes_with_distance_head[distance]].equi_distance_nodes);
	} else {
		INIT_LIST_HEAD(&node->equi_distance_nodes);
		queue->nodes_with_distance_head[distance] = node->vertex_num;
	}

	queue->max_distance_ever_seen = MAX(queue->max_distance_ever_seen, distance);
};

static inline Node *Queue__pop_min(Queue *queue, Graph *graph)
{
	while (queue->current_cell_num <= queue->max_distance_ever_seen) {
		Vertex_Num *current_cell = &queue->nodes_with_distance_head[queue->current_cell_num];
		Vertex_Num vertex_num = *current_cell;

		if (vertex_num != INVALID_VERTEX) {
			Node *node = &graph->nodes[vertex_num];

			if (list_empty(&node->equi_distance_nodes)) {
				*current_cell = INVALID_VERTEX;
				queue->current_cell_num++;
			} else {
				*current_cell = list_entry(node->equi_distance_nodes.next, Node, equi_distance_nodes);
				list_del_init(&node->equi_distance_nodes);
			}
			return node;
		}
		queue->current_cell_num++;
	}

	return NULL;
};

static inline void Queue__decrease_key(Queue *queue, Node *node, Distance new_distance, Graph *graph)
{
	Vertex_Num *original_cell = &queue->nodes_with_distance_head[node->distance];
	Vertex_Num *new_cell = &queue->nodes_with_distance_head[new_distance];

	if (list_empty(&node->equi_distance_nodes)) {
		*original_cell = INVALID_VERTEX;
	} else {
		list_del_init(&node->equi_distance_nodes);
	}

	if (*new_cell != INVALID_VERTEX) {
		struct list_head *head = &graph->nodes[*new_cell].equi_distance_nodes;
		list_add(&node->equi_distance_nodes, head);
	} else {
		*new_cell = node->vertex_num;
	}
}

#endif
