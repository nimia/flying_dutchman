#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include "../global.h"

typedef struct Binary_Heap Binary_Heap;
typedef Binary_Heap Queue;

struct Queue_Element_Pointer;
typedef struct Queue_Element_Pointer Queue_Element_Pointer;

struct Binary_Heap {
	Vertex_And_Distance _array[NUM_OF_VERTICES];
	Queue_Element_Pointer *_queue_elements;
};

struct Queue_Element_Pointer {
	Vertex _index;
};

#include "../queue.h"

#endif
