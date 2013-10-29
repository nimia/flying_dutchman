#ifndef QUEUE_INTERNAL_H_
#define QUEUE_INTERNAL_H_

#include <stdlib.h>

#include "../list.h"
#include "../graph.h"
#include "../utils.h"

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

#define Queue__CHUNK_SIZE_IN_BITS 16
#define Queue__CHUNK_SIZE (1 << Queue__CHUNK_SIZE_IN_BITS)
#define Queue__NUM_OF_CHUNKS (((int64_t)1 << 32) / Queue__CHUNK_SIZE)

typedef int32_t Chunk;
#define Chunk__INVALID (-1)

typedef int32_t Cell_Inside_Chunk;

typedef struct Queue {
	Vertex_Num equi_distance_vertices_head_vertex_num[Queue__CHUNK_SIZE];
	Chunk current_chunk;

	Vertex_Num chunk_heads[Queue__NUM_OF_CHUNKS];
	Cell_Inside_Chunk min_distance_candidate;

	// this does NOT decrement on decrease_key for code simplicity purposes; it's really "max ever seen"
	// it isn't of type Distance to avoid any over/under flow
	Chunk max_chunk_ever_used;
} Queue;

static inline void Queue__init(Queue *queue)
{
	for (int64_t i = 0; i < Queue__CHUNK_SIZE; i++) {
		queue->equi_distance_vertices_head_vertex_num[i] = Vertex__INVALID;
	}
	queue->current_chunk = 0;

	for (int64_t i = 0; i < Queue__NUM_OF_CHUNKS; i++) {
		queue->chunk_heads[i] = Vertex__INVALID;
	}

	queue->min_distance_candidate = 0;
	queue->max_chunk_ever_used = (Chunk){0};
}

static inline Chunk Queue__get_chunk(Distance distance)
{
	return (distance & ~(Queue__CHUNK_SIZE - 1)) >> Queue__CHUNK_SIZE_IN_BITS;
}

static inline Cell_Inside_Chunk Queue__get_cell_inside_chunk(Distance distance)
{
	return distance & (Queue__CHUNK_SIZE - 1);
}

static inline void Queue__insert_to_something(Vertex_Num *vertex_nump, Vertex *vertex, struct Graph *graph)
{
	Vertex_Num vertex_num = *vertex_nump;

	if (vertex_num != Vertex__INVALID) {
		list_add(&vertex->queue_data.equi_distance_vertices,
				 &graph->vertices[vertex_num].queue_data.equi_distance_vertices);
	} else {
		INIT_LIST_HEAD(&vertex->queue_data.equi_distance_vertices);
		*vertex_nump= vertex->vertex_num;
	}
}

static inline void Queue__delete(Queue *queue, Vertex *vertex, Graph *graph);

static inline void Queue__insert(Queue *queue, Vertex *vertex, Distance distance, struct Graph *graph)
{
	DEBUG("inserting %d with distance %d\n", vertex->vertex_num, distance);

	Chunk chunk = Queue__get_chunk(distance);
	Cell_Inside_Chunk cell = Queue__get_cell_inside_chunk(distance);

	if (vertex->vertex_num == 3067611) {
		printf("inserting problem vertex, chunk is %d, cell is %d, distance is %d\n",
				chunk, cell, distance);
	}

	if (queue->max_chunk_ever_used < chunk) {
		for (int64_t i = queue->max_chunk_ever_used + 1; i <= chunk; i++) {
			queue->chunk_heads[i] = Vertex__INVALID;
		}

		queue->max_chunk_ever_used = chunk;
	}

	vertex->distance = distance;
	if (queue->current_chunk < chunk) {
		Queue__insert_to_something(&queue->chunk_heads[chunk], vertex, graph);
	} else {
		Queue__insert_to_something(&queue->equi_distance_vertices_head_vertex_num[cell], vertex, graph);
	}
	DEBUG("done inserting %d with distance %d\n", vertex->vertex_num, distance);
}

static inline void Queue__delete_something(Vertex_Num *vertex_nump, Vertex *vertex, Queue *queue, Graph *graph)
{
	if (list_empty(&vertex->queue_data.equi_distance_vertices)) {
		assert(queue->equi_distance_vertices_head_vertex_num[6047] != 3067611);
		*vertex_nump = Vertex__INVALID;
		assert(queue->equi_distance_vertices_head_vertex_num[6047] != 3067611);
	} else {
		assert(queue->equi_distance_vertices_head_vertex_num[6047] != 3067611);
		assert(graph->vertices[1056364].queue_data.equi_distance_vertices.next != &graph->vertices[3067611].queue_data.equi_distance_vertices);
		Vertex *new_head = list_entry(vertex->queue_data.equi_distance_vertices.next,
									  Vertex, queue_data.equi_distance_vertices);

		//printf("new_head->vertex_num is %d\n", new_head->vertex_num);
		*vertex_nump = new_head->vertex_num;
		list_del_init(&vertex->queue_data.equi_distance_vertices);
		assert(queue->equi_distance_vertices_head_vertex_num[6047] != 3067611);
	}
}

static inline void Queue__delete(Queue *queue, Vertex *vertex, Graph *graph)
{
	if (vertex->vertex_num == 3067611) {
		printf("deleting problem vertex, chunk is %d, cell is %d, distance is %d\n",
				queue->current_chunk, queue->min_distance_candidate, vertex->distance);
	}

	Distance distance = vertex->distance;
	Chunk chunk = Queue__get_chunk(distance);
	Cell_Inside_Chunk cell = Queue__get_cell_inside_chunk(distance);

	if (chunk == queue->current_chunk) {
		Queue__delete_something(&queue->equi_distance_vertices_head_vertex_num[cell], vertex, queue, graph);
	} else {
		Queue__delete_something(&queue->chunk_heads[chunk], vertex, queue, graph);
	}
}

static inline Vertex *Queue__pop_min_inside_current_chunk(Queue *queue, Graph *graph)
{
	static bool_t visited = FALSE;

	while (queue->min_distance_candidate < Queue__CHUNK_SIZE) {
		Vertex_Num *equi_distance_vertices_head_vertex_nump =
				&queue->equi_distance_vertices_head_vertex_num[queue->min_distance_candidate];

		Vertex_Num equi_distance_vertices_head_vertex_num = *equi_distance_vertices_head_vertex_nump;

		if (equi_distance_vertices_head_vertex_num != Vertex__INVALID) {
			Vertex *vertex = &graph->vertices[equi_distance_vertices_head_vertex_num];
			if (vertex->vertex_num == 3067611) {
				if (visited) {
					printf("popping problem vertex for the 2nd time, chunk is %d, cell is %d, distance is %d\n",
							queue->current_chunk, queue->min_distance_candidate, vertex->distance);
					exit(1);
				} else {
					visited = TRUE;
					printf("popping problem vertex for the 1st time, chunk is %d, cell is %d, distance is %d\n",
							queue->current_chunk, queue->min_distance_candidate, vertex->distance);
				}
			}
			Queue__delete(queue, vertex, graph);
			DEBUG("popped min inside current chunk %d with distance %d\n", vertex->vertex_num, vertex->distance);
			return vertex;
		}
		queue->min_distance_candidate++;
	}

	return NULL;
}

static inline Chunk Queue__find_next_nonempty_chunk(Queue *queue)
{
	for (Chunk i = queue->current_chunk + 1; i < Queue__NUM_OF_CHUNKS; i++) {
		if (queue->chunk_heads[i] != Vertex__INVALID) {
			return i;
		}
	}
	return Chunk__INVALID;
}

static inline void Queue__assert_all_cells_are_empty(Queue *queue)
{
	for (int i = 0; i < Queue__CHUNK_SIZE; i++) {
		assert(queue->equi_distance_vertices_head_vertex_num[i] == Vertex__INVALID);
	}
}

static inline void Queue__spill_chunk(Queue *queue, Chunk chunk, Graph *graph)
{
	queue->current_chunk = chunk;
	queue->min_distance_candidate = 0;

	Queue__assert_all_cells_are_empty(queue);

	if (chunk == 12) {
		printf("spilling chunk 12, all cells are empty\n");
		assert(queue->equi_distance_vertices_head_vertex_num[6047] == Vertex__INVALID);
	}

	Vertex *representative = &graph->vertices[queue->chunk_heads[chunk]];
	Vertex *v, *temp;
	list_for_each_entry_safe(v, temp, &representative->queue_data.equi_distance_vertices, queue_data.equi_distance_vertices) {
		Cell_Inside_Chunk cell = Queue__get_cell_inside_chunk(v->distance);
		Queue__insert_to_something(&queue->equi_distance_vertices_head_vertex_num[cell], v, graph);
	}
	v = representative;
	Cell_Inside_Chunk cell = Queue__get_cell_inside_chunk(v->distance);
	Queue__insert_to_something(&queue->equi_distance_vertices_head_vertex_num[cell], v, graph);
}

static inline Vertex *Queue__pop_min(Queue *queue, Graph *graph)
{
	Vertex *result = Queue__pop_min_inside_current_chunk(queue, graph);
	if (result) {
		return result;
	}

	Chunk chunk = Queue__find_next_nonempty_chunk(queue);
	if (chunk == Chunk__INVALID) {
		return NULL;
	}

	Queue__spill_chunk(queue, chunk, graph);
	return Queue__pop_min_inside_current_chunk(queue, graph);
}

static inline void Queue__decrease_key(Queue *queue, Vertex *vertex, Distance new_distance, Graph *graph)
{
	if (vertex->vertex_num == 3067611) {
		printf("decreasing key of problem vertex from %d to %d\n", vertex->distance, new_distance);
		if (vertex->distance == DISTANCE__INFINITY) {
			printf("previous distance was infinity\n");
		}
	}

	assert(queue->equi_distance_vertices_head_vertex_num[6047] != 3067611);

	if (vertex->distance != DISTANCE__INFINITY) {
		Queue__delete(queue, vertex, graph);
	}

	if (queue->equi_distance_vertices_head_vertex_num[6047] == 3067611) {
		printf("BUG\n");
		abort();
	}

	Queue__insert(queue, vertex, new_distance, graph);
}

#endif
