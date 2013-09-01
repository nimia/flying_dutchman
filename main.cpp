#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "queue_test.h"

#include "list.h"
#include "graph.h"
#include "queue.h"
#include "utils.h"
#include "dijkstra.h"

bool_t parse_line(char *line, char delimiter, Vertex_Num *first, Vertex_Num *second, Distance *distance)
{
	if (!strcmp(line, "\r\n") || !strcmp(line, "\n")) {
		return FALSE;
	}

	char *start = line;
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

void reset_graph(Graph *graph)
{
	for (int i = 0; i < VERTEX__MAX_NUM_OF_VERTICES; i++) {
		Vertex *vertex = &graph->vertexs[i];
		INIT_LIST_HEAD(&vertex->equi_distance_vertexs);
		vertex->neighbors.clear();
		vertex->distance = DISTANCE_INFINITY;
		vertex->vertex_num = i;
	}

	graph->max_vertex_num = -1;
}

void load_graph(char *filename, char delimiter, Graph *graph)
{
	reset_graph(graph);

	FILE *f = fopen(filename, "r");
	char line[128];

	while (fgets(line, sizeof line, f) != NULL ) {
		Vertex_Num first, second;
		Distance distance;

		if (parse_line(line, delimiter, &first, &second, &distance)) {
			if (MAX(first, second) >= VERTEX__MAX_NUM_OF_VERTICES) {
				printf("Encountered vertex with too big an index - did you remember to set NUM_OF_VERTICES?\n");
				abort();
			}

			graph->max_vertex_num = MAX(graph->max_vertex_num, MAX(first, second));

			Vertex *first_vertex = &graph->vertexs[first];
			Vertex *second_vertex = &graph->vertexs[second];
			first_vertex->neighbors.push_front({second_vertex, distance});
			DEBUG("%d => %d, distance %d\n", first, second, distance);
		}
	}

	fclose(f);
}

void print_distances(Graph *graph)
{
	for (int i = 0; i <= graph->max_vertex_num; i++) {
		DEBUG("%d %d\n", i, graph->vertexs[i].distance);
	}
}

Queue the_queue;
Graph the_graph;

int main(int argc, char *argv[])
{
	if (argc == 2 && !strcmp(argv[1], "test")) {
		run_all_tests();
		exit(0);
	}

	load_graph("example", ' ', &the_graph);
	dijkstra(&the_graph, 1, &the_queue);
	print_distances(&the_graph);
}
