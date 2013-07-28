#ifndef GLOBAL_H
#define GLOBAL_H

#define __STDC_LIMIT_MACROS
#include <stdint.h>

typedef uint8_t Distance;
#define DISTANCE_INFINITY (UINT8_MAX)

#define NUM_OF_VERTICES 36692
typedef uint16_t Vertex;
#define INVALID_VERTEX (UINT16_MAX)

struct Vertex_And_Distance {
	Vertex _vertex;
	Distance _distance;
};

#endif
