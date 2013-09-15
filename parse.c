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

bool_t parse_simple_space_delimited_line(char *line, Vertex_Num *first, Vertex_Num *second, Distance *distance)
{
	if (!strcmp(line, "\r\n") || !strcmp(line, "\n")) {
		return FALSE;
	}

	return parse_delimiter_based_line(line, ' ', first, second, distance, 0);
}
