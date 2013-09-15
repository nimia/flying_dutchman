#ifndef QUEUE_H
#define QUEUE_H

typedef struct Queue Queue;

static void Queue__init(Queue *queue);

static void Queue__insert(Queue *queue, Vertex *vertex, Distance distance, struct Graph *graph);

static Vertex *Queue__pop_min(Queue *queue, Graph *graph);

static void Queue__decrease_key(Queue *queue, Vertex *vertex, Distance new_distance, Graph *graph);

#include <queue_internal.h>

#endif
