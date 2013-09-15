#include <time.h>
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
#include "parse.h"

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
	clock_t start, end;

	the_queue = (Queue *)malloc(sizeof(Queue));

	if (argc == 2 && !strcmp(argv[1], "test")) {
		run_build_tests();
		exit(0);
	}
	if (argc == 2 && !strcmp(argv[1], "extended")) {
		run_all_tests();
		exit(0);
	}

	Vertex_Num starting_vertex;
	if (argc == 2 && !strcmp(argv[1], "example")) {
		load_graph("example.graph", &parse_simple_space_delimited_line, &the_graph);
		starting_vertex = 1;
	} else if (argc == 3) {
		load_graph(argv[1], &parse_usa_challenge_line, &the_graph);
		starting_vertex = atoi(argv[2]);
	} else {
		printf("Not sure what you want, check your arguments\n");
		exit(1);
	}

	start = clock();
	printf("Starting Dijkstra\n");
	dijkstra(&the_graph, starting_vertex, the_queue);
	end = clock();

//	print_distances(&the_graph);
	printf("Dijkstra took %ld seconds\n", (end - start) / CLOCKS_PER_SEC);
}
