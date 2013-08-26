#ifndef SIZED_INTS_H
#define SIZED_INTS_H

#define __STDC_LIMIT_MACROS
#include <stdint.h>

typedef uint16_t Distance;
#define DISTANCE_INFINITY (UINT16_MAX)
#define DISTANCE_MAX DISTANCE_INFINITY
#define DISTANCE_NUM_OF_POSSIBLE_VALUES (DISTANCE_INFINITY + 1)

#define NUM_OF_VERTICES 36692
typedef uint32_t Vertex_Num;
#define INVALID_VERTEX (UINT32_MAX)

#endif
