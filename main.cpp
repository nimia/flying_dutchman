#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "queue_test.h"

#include "list.h"
#include "graph.h"
#include "queue.h"
#include "utils.h"
#include "dijkstra.h"
#include "bellman_ford.h"

typedef bool_t line_parse_func_t(char *line, Vertex_Num *first, Vertex_Num *second, Distance *distance);

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

bool_t parse_boost_output_line(char *line, Vertex_Num *first, Vertex_Num *second, Distance *distance)
{
	// last 2 lines are "Running...", those are the timing results
	if (line[0] == 'R') {
		return FALSE;
	}

	char *start = line + 1;
	char *end = strchr(start, ',');
	*end = '\0';
	*first = atoi(start);

	start = end + 1;
	end = strchr(start, ')');
	*end = '\0';
	*second = atoi(start);

	start = end + 2;
	*distance = atoi(start);

	return TRUE;
}

void reset_graph(Graph *graph)
{
	for (int i = 0; i < VERTEX__MAX_NUM_OF_VERTICES; i++) {
		Vertex *vertex = &graph->vertices[i];
		INIT_LIST_HEAD(&vertex->equi_distance_vertices);
		vertex->edges.clear();
		vertex->distance = DISTANCE__INFINITY;
		vertex->vertex_num = i;
	}

	graph->max_vertex_num = -1;
}

void load_graph(char *filename, line_parse_func_t *parse_line, Graph *graph)
{
	reset_graph(graph);

	FILE *f = fopen(filename, "r");
	char line[128];

	Vertex *vertex_with_most_edges = &graph->vertices[0];
	while (fgets(line, sizeof line, f) != NULL ) {
		Vertex_Num first, second;
		Distance distance;

		if ((*parse_line)(line, &first, &second, &distance)) {
			if (MAX(first, second) >= VERTEX__MAX_NUM_OF_VERTICES) {
				printf("Encountered vertex with too big an index - did you remember to set NUM_OF_VERTICES?\n");
				abort();
			}

			graph->max_vertex_num = MAX(graph->max_vertex_num, MAX(first, second));

			Vertex *first_vertex = &graph->vertices[first];
			Vertex *second_vertex = &graph->vertices[second];
			first_vertex->edges.push_front({second_vertex, distance});

			if (first_vertex->edges.size() > vertex_with_most_edges->edges.size()) {
				vertex_with_most_edges = first_vertex;
			}

			DEBUG("%d => %d, distance %d\n", first, second, distance);
		}
	}

	printf("Vertex with most edges is %d\n", vertex_with_most_edges->vertex_num);
	fclose(f);
}

void print_distances(Graph *graph)
{
	int num_of_covered_vertices = 0;
	Distance max_distance = 0;

	for (int i = 0; i <= graph->max_vertex_num; i++) {
		Distance distance = graph->vertices[i].distance;

		if (distance != DISTANCE__INFINITY) {
			printf("%d %d\n", i, distance);
			num_of_covered_vertices++;
			max_distance = MAX(max_distance, distance);
		}
	}

	printf("Covered %d vertices, max distance is %d\n", num_of_covered_vertices, max_distance);
}

Queue *the_queue;
Graph the_graph;

int main(int argc, char *argv[])
{
	the_queue = (Queue *)malloc(sizeof(Queue));

	if (argc == 2 && !strcmp(argv[1], "test")) {
		run_build_tests();
		exit(0);
	}
	if (argc == 2 && !strcmp(argv[1], "extended")) {
		run_all_tests();
		exit(0);
	}

	if (argc == 2 && !strcmp(argv[1], "example")) {
		load_graph("example", &parse_simple_space_delimited_line, &the_graph);
		dijkstra(&the_graph, 1, the_queue);
	} else if (argc == 3 && !strcmp(argv[1], "--boost")) {
		load_graph(argv[2], &parse_boost_output_line, &the_graph);
		dijkstra(&the_graph, 0, the_queue);
	} else if (argc == 3 && !strcmp(argv[1], "--bf")) {
		load_graph(argv[2], &parse_boost_output_line, &the_graph);
		bellman_ford(&the_graph, 0);
	} else if (argc == 2) {
		load_graph(argv[1], &parse_usa_challenge_line, &the_graph);
		dijkstra(&the_graph, atoi(argv[2]), the_queue);
	} else {
		printf("Not sure what you want, check your arguments\n");
		exit(1);
	}
	print_distances(&the_graph);
}
