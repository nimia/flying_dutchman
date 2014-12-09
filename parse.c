#include <stdlib.h>
#include <string.h>

#include "parse.h"

bool_t parse_delimiter_based_line(char *line, char delimiter, Vertex_Num *first, Vertex_Num *second, Distance *distance,
		int skip_chars_in_line_start)
{
	char *start = line + skip_chars_in_line_start;
	char *end = strchr(start, delimiter);
	*end = '\0';
	*first = atoi(start);

	start = end + 1;
	end = strchr(start, delimiter);
	*end = '\0';
	*second = atoi(start);

	start = end + 1;
	*distance = atoi(start);

	return TRUE;
}

bool_t parse_usa_challenge_line(char *line, Vertex_Num *first, Vertex_Num *second, Distance *distance)
{
	if (!strcmp(line, "\r\n") || !strcmp(line, "\n") || line[0] == 'c' || line[0] == 'p') {
		return FALSE;
	}

	return parse_delimiter_based_line(line, ' ', first, second, distance, 2);
}

bool_t parse_proteins_line(char *line, Vertex_Num *first, Vertex_Num *second, Distance *distance)
{
	if (!strcmp(line, "\r\n") || !strcmp(line, "\n") || line[0] == 'c' || line[0] == 'p') {
		return FALSE;
	}

	return parse_delimiter_based_line(line, ' ', first, second, distance, 0);
}

bool_t parse_ba_line(char *line, Vertex_Num *first, Vertex_Num *second, Distance *distance)
{
	return parse_delimiter_based_line(line, ' ', first, second, distance, 0);
}

bool_t parse_p2p_line(char *line, Vertex_Num *first, Vertex_Num *second, Distance *distance)
{
	char *start = line;
	char *end = strchr(start, ' ');
	*end = '\0';
	*first = atoi(start);

	start = end + 1;
	end = strchr(start, ' ');
	*end = '\0';
	*second = atoi(start);

	start = end + 1;
	float distance_float = atof(start);
	*distance = (int)(distance_float * 10000);

	return TRUE;
}

bool_t parse_simple_space_delimited_line(char *line, Vertex_Num *first, Vertex_Num *second, Distance *distance)
{
	if (!strcmp(line, "\r\n") || !strcmp(line, "\n")) {
		return FALSE;
	}

	return parse_delimiter_based_line(line, ' ', first, second, distance, 0);
}

bool_t parse_boost_line(char *line, Vertex_Num *first, Vertex_Num *second, Distance *distance)
{
	if (line[0] != '(') {
		return FALSE;
	}

	char *comma = strchr(line, ',');
	*comma = '\0';
	*first = atoi(line + 1);

	char *parens_close = strchr(comma + 1, ')');
	*parens_close = '\0';
	*second = atoi(comma + 1);

	*distance = atoi(parens_close + 2);

	return TRUE;
}
