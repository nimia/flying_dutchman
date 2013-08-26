#ifndef QUEUE_H
#define QUEUE_H

typedef struct Queue Queue;

void Queue__init(Queue *queue);

void Queue__insert(Queue *queue, Node *node, Distance distance, struct Graph *graph);

Node *Queue__pop_min(Queue *queue, Graph *graph);

void Queue__decrease_key(Queue *queue, Node *node, Distance new_distance, Graph *graph);

#include "queue_internal.h"

#endif
