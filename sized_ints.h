#ifndef SIZED_INTS_H
#define SIZED_INTS_H

#define __STDC_LIMIT_MACROS
#include <stdint.h>

typedef uint32_t Distance;
#define DISTANCE_INFINITY (UINT32_MAX - 2)
#define DISTANCE_MAX DISTANCE_INFINITY
#define DISTANCE_NUM_OF_POSSIBLE_VALUES (DISTANCE_INFINITY + 1)

#define VERTEX__MAX_NUM_OF_VERTICES 321271
typedef int32_t Vertex_Num;
#define VERTEX__INVALID_VERTEX (INT32_MAX)

#endif
