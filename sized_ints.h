#ifndef SIZED_INTS_H
#define SIZED_INTS_H

#define __STDC_LIMIT_MACROS
#include <stdint.h>

typedef uint32_t Distance;
#define DISTANCE__INFINITY (UINT32_MAX - 2)
#define DISTANCE__QUEUE_QUICK_INIT_PREFIX_LENGTH 1000000
#define DISTANCE__MAX DISTANCE__INFINITY
#define DISTANCE__NUM_OF_POSSIBLE_VALUES (DISTANCE__INFINITY + 1)

typedef int32_t Vertex_Num;
#define Vertex__INVALID (INT32_MAX)

#endif
