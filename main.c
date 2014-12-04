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

void load_graph(char *filename, line_parse_func_t *parse_line, Graph *graph, bool_t bidirectional_edges)
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
			if (bidirectional_edges) {
				Graph__add_edge(graph, second_vertex, first_vertex, distance);
			}

			DEBUG("%d => %d, distance %d\n", first, second, distance);
		}
	}

	fclose(f);
}

FILE *output_file;

void print_distances(Graph *graph)
{
	int num_of_covered_vertices = 0;
	Distance max_distance = 0;

	for (int i = 0; i <= graph->max_vertex_num; i++) {
		Distance distance = graph->vertices[i].distance;

		if (distance != DISTANCE__INFINITY) {
			fprintf(output_file, "%d %d\n", i, distance);
			num_of_covered_vertices++;
			max_distance = MAX(max_distance, distance);
		}
	}

	fprintf(output_file, "Covered %d vertices, max distance is %d\n", num_of_covered_vertices, max_distance);
}

typedef void (*Algorithm)(Graph *, Vertex_Num, Queue*);

char *algorithm_name(Algorithm algorithm)
{
	if (algorithm == &bellman_ford) {
		return "Bellman-Ford";
	}
	if (algorithm == &dijkstra) {
		return "Dijkstra";
	}
	abort();
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

	Vertex_Num starting_vertex = atoi(argv[3]);
	Algorithm algorithm;

	char output_file_path[256]; //yuck
	line_parse_func_t *parse_line;

#define RESULTS_PATH "/localwork/dijkstra"

	bool_t bidirectional_edges = 100;
	bool_t summary = FALSE;
	if (!strcmp(argv[1], "bf")) {
		algorithm = &bellman_ford;
		sprintf(output_file_path, RESULTS_PATH"/bf_results_on_");
		parse_line = &parse_usa_challenge_line;
		bidirectional_edges = FALSE;
	} else if (!strcmp(argv[1], "dijkstra")) {
		algorithm = &dijkstra;
		sprintf(output_file_path, RESULTS_PATH"/my_results_on_");
		parse_line = &parse_usa_challenge_line;
		bidirectional_edges = FALSE;
	} else if (!strcmp(argv[1], "proteins")) {
		printf("sizeof(Graph) = %lu\n", sizeof(Graph));
		printf("sizeof(Queue) = %lu\n", sizeof(Queue));
		algorithm = &dijkstra;
		sprintf(output_file_path, RESULTS_PATH"/my_results_on_");
		parse_line = &parse_proteins_line;
		bidirectional_edges = FALSE;
	} else if (!strcmp(argv[1], "summary")) {
		algorithm = &dijkstra;
		summary = TRUE;
		srand(time(NULL));
		starting_vertex = (rand() % 23000000) + 1;
		printf("%d", starting_vertex);
		fflush(stdout);
		sprintf(output_file_path, RESULTS_PATH"/my_results_on_");
		parse_line = &parse_usa_challenge_line;
		bidirectional_edges = FALSE;
	} else if (!strcmp(argv[1], "summary-proteins")) {
		algorithm = &dijkstra;
		summary = TRUE;
		srand(time(NULL));
		starting_vertex = (rand() % 4000000) + 1;
		printf("%d", starting_vertex);
		fflush(stdout);
		sprintf(output_file_path, RESULTS_PATH"/my_results_on_");
		parse_line = &parse_proteins_line;
		bidirectional_edges = FALSE;
	} else if (!strcmp(argv[1], "er")) {
		algorithm = &dijkstra;
		sprintf(output_file_path, RESULTS_PATH"/my_results_on_");
		parse_line = &parse_boost_line;
		bidirectional_edges = FALSE;
	} else if (!strcmp(argv[1], "p2p")) {
		algorithm = &dijkstra;
		sprintf(output_file_path, RESULTS_PATH"/my_results_on_");
		parse_line = &parse_p2p_line;
		bidirectional_edges = TRUE;
	} else {
		printf("Not sure what you want, check your arguments\n");
		exit(1);
	}
	strcat(output_file_path, my_basename(argv[2]));
	strcat(output_file_path, "_");
	strcat(output_file_path, argv[3]);
	if (bidirectional_edges == 100) {
		printf("Did you forget to specify bidirectional_edges?\n");
		exit(1);
	}
	load_graph(argv[2], parse_line, &the_graph, bidirectional_edges);

	if (!summary) {
		output_file = fopen(output_file_path, "w");
		fprintf(output_file, "Starting algorithm %s on graph %s, from starting vertex %d\n",
				algorithm_name(algorithm), argv[2], starting_vertex);
	}

	clock_t start = clock();
	(*algorithm)(&the_graph, starting_vertex, the_queue);
	clock_t end = clock();

	if (summary) {
		printf(" %f\n", (float)(end - start) / CLOCKS_PER_SEC);
		exit(0);
	}
	fprintf(output_file, "Algorithm took %f seconds\n", (float)(end - start) / CLOCKS_PER_SEC);
	print_distances(&the_graph);

	fflush(output_file);
	fclose(output_file);
}
