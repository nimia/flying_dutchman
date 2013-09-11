#ifndef TEST_UTIL_H_
#define TEST_UTIL_H_

#include "graph.h"
#include "queue.h"

extern Queue *the_queue;
extern Graph the_graph;

typedef bool_t line_parse_func_t(char *line, Vertex_Num *first, Vertex_Num *second, Distance *distance);

bool_t parse_simple_space_delimited_line(char *line, Vertex_Num *first, Vertex_Num *second, Distance *distance);

void load_graph(char *filename, line_parse_func_t *parse_line, Graph *graph);

#endif
