#ifndef TRIE_H
#define TRIE_H

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/trie_policy.hpp>
#include "../global.h"

typedef struct Queue{
	__gnu_pbds::trie<int, int> trie;
} Queue;

#include "../queue.h"

//void Queue__init(Queue *queue);
//void Queue__insert(Queue *queue, Vertex v, Distance d);
//bool Queue__is_empty(Queue *queue);
//Vertex_And_Distance Queue__pop_min(Queue *queue);
//void Queue__decrease_key(Queue *queue, Vertex v, Distance d);


#endif
