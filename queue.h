#ifndef QUEUE_H
#define QUEUE_H

#include "global.h"

struct Queue_Element_Pointer;
typedef struct Queue_Element_Pointer Queue_Element_Pointer;

void Queue__init(Queue *queue, Queue_Element_Pointer queue_elements[NUM_OF_VERTICES]);
void Queue__insert(Queue *queue, Vertex v, Distance d);
bool Queue__empty(Queue *queue);
Vertex Queue__pop_min(Queue *queue);
void Queue__decrease_key(Queue *queue, Queue_Element_Pointer p, Distance d);

#endif
