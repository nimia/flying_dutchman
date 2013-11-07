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

void load_graph(char *filename, line_parse_func_t *parse_line, Graph *graph)
{
	Graph__reset(graph);

	FILE *f = fopen(filename, "r");
	char line[128];

	while (fgets(line, sizeof line, f) != NULL ) {
		Vertex_Num first, second;
		Distance distance;

		if ((*parse_line)(line, &first, &second, &distance)) {
			if (MAX(first, second) >= GRAPH__MAX_NUM_OF_VERTICES) {
				printf("Encountered vertex with too big an index - did you remember to set NUM_OF_VERTICES?\n");
				abort();
			}

			graph->max_vertex_num = MAX(graph->max_vertex_num, MAX(first, second));

			Vertex *first_vertex = &graph->vertices[first];
			Vertex *second_vertex = &graph->vertices[second];
			Graph__add_edge(graph, first_vertex, second_vertex, distance);

//			DEBUG("%d => %d, distance %d\n", first, second, distance);
		}
	}

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

void assert_example_distances_are_correct(Graph *graph)
{
#define ASSERT_DISTANCE(_vertex_num, _distance) do { assert(graph->vertices[(_vertex_num)].distance == (_distance)); } while (0)
	ASSERT_DISTANCE(1, 0);
	ASSERT_DISTANCE(2, 7);
	ASSERT_DISTANCE(3, 7);
	ASSERT_DISTANCE(4, 17);
	ASSERT_DISTANCE(5, 16);
	ASSERT_DISTANCE(6, 676498121 + 16);
#undef ASSERT_DISTANCE
	printf("Example distances are correct\n");
}

int main(int argc, char *argv[])
{
	printf("sizeof(Queue) = %lu\n", sizeof(Queue));
	printf("sizeof(Graph) = %lu\n", sizeof(Graph));

	clock_t start, end;

	the_queue = (Queue *)malloc(sizeof(Queue));

	if (argc == 2 && !strcmp(argv[1], "extended")) {
		run_all_tests();
		exit(0);
	}

	Vertex_Num starting_vertex;
	void (*algorithm)(Graph *, Vertex_Num, Queue*);

	if (!strcmp(argv[1], "bf")) {
		load_graph(argv[2], &parse_usa_challenge_line, &the_graph);
		algorithm = &bellman_ford;
		starting_vertex = atoi(argv[3]);
	} else if (argc == 2 && !strcmp(argv[1], "example")) {
		load_graph("example.graph", &parse_simple_space_delimited_line, &the_graph);
		dijkstra(&the_graph, 1, the_queue);
		assert_example_distances_are_correct(&the_graph);
		return 0;
	} else if (argc == 3) {
		load_graph(argv[1], &parse_usa_challenge_line, &the_graph);
		algorithm = &dijkstra;
		starting_vertex = atoi(argv[2]);
	} else {
		printf("Not sure what you want, check your arguments\n");
		exit(1);
	}

	start = clock();
	printf("Starting algorithm\n");
	(*algorithm)(&the_graph, starting_vertex, the_queue);
	end = clock();

	print_distances(&the_graph);
	printf("Algorithm took %f seconds\n", ((float)(end - start)) / CLOCKS_PER_SEC);
}
