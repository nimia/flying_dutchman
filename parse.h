#ifndef PARSE_H_
#define PARSE_H_

#include "utils.h"
#include "sized_ints.h"

typedef bool_t line_parse_func_t(char *line, Vertex_Num *first, Vertex_Num *second, Distance *distance);

bool_t parse_simple_space_delimited_line(char *line, Vertex_Num *first, Vertex_Num *second, Distance *distance);

bool_t parse_usa_challenge_line(char *line, Vertex_Num *first, Vertex_Num *second, Distance *distance);

bool_t parse_boost_line(char *line, Vertex_Num *first, Vertex_Num *second, Distance *distance);

#endif
