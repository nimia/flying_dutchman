#ifndef TEST_UTIL_H_
#define TEST_UTIL_H_

#include "graph.h"
#include "queue.h"

extern Queue the_queue;
extern Graph the_graph;

void load_graph(char *filename, char delimiter, Graph *graph);

#endif
