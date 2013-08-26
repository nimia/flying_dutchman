#ifndef GLOBAL_H
#define GLOBAL_H

#define __STDC_LIMIT_MACROS
#include <stdint.h>

#include <list>
using namespace std;

typedef uint16_t Distance;
#define DISTANCE_INFINITY (UINT16_MAX)
#define DISTANCE_MAX DISTANCE_INFINITY
#define DISTANCE_NUM_OF_POSSIBLE_VALUES (DISTANCE_INFINITY + 1)

#define NUM_OF_VERTICES 36692
typedef uint32_t Vertex_Num;
#define INVALID_VERTEX (UINT32_MAX)

typedef struct Node Node;

typedef struct Node_And_Distance {
	Node *node;
	Distance distance;
} Node_And_Distance;

typedef struct Node {
	struct list_head equi_distance_nodes;
	list<Node_And_Distance> neighbors;
	Distance distance;
	Vertex_Num vertex_num;
} Node;

typedef struct Graph {
	Node nodes[NUM_OF_VERTICES];
} Graph;

#define ASSERT(x) do { if (!(x)) { printf("ASSERT failed, %s, line %d\n", __FILE__, __LINE__);}} while(0)

typedef enum bool_t {
	FALSE = 0,
	TRUE = 1
} bool_t;

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define DEBUG_PRINTS 1
#define DEBUG(fmt, ...) \
            do { if (DEBUG_PRINTS) printf(fmt, __VA_ARGS__); } while (0)

#endif
