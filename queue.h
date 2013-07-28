#ifndef QUEUE_H
#define QUEUE_H

#include "global.h"

void Queue__init(Queue *queue);
void Queue__insert(Queue *queue, Vertex v, Distance d);
bool Queue__is_empty(Queue *queue);
Vertex_And_Distance Queue__pop_min(Queue *queue);
void Queue__decrease_key(Queue *queue, Vertex v, Distance d);

#endif
