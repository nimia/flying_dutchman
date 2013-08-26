#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void load_graph(char *filename, char delimiter, Graph *graph)
{
	for (int i = 0; i < NUM_OF_VERTICES; i++) {
		Node *node = &graph->nodes[i];
		INIT_LIST_HEAD(&node->equi_distance_nodes);
		node->neighbors.clear();
		node->distance = DISTANCE_INFINITY;
		node->vertex_num = i;
	}

	FILE *f = fopen(filename, "r");
	char line[128];

	while (fgets(line, sizeof line, f) != NULL ) {
		Vertex_Num first, second;
		Distance distance;

		if (parse_line(line, delimiter, &first, &second, &distance)) {
			if (first >= NUM_OF_VERTICES || second >= NUM_OF_VERTICES) {
				printf("Encountered vertex with too big an index - did you remember to set NUM_OF_VERTICES?\n");
				abort();
			}

			Node *first_node = &graph->nodes[first];
			Node *second_node = &graph->nodes[second];
			first_node->neighbors.push_front({second_node, distance});
			DEBUG("%d => %d, distance %d\n", first, second, distance);
		}
	}

	fclose(f);
}

void print_distances(Graph *graph)
{
	for (int i = 1; i < 5; i++) {
		DEBUG("%d %d\n", i, graph->nodes[i].distance);
	}
}

Queue the_queue;
Graph the_graph;

int main()
{
	load_graph("example", ' ', &the_graph);
	dijkstra(&the_graph, 1, &the_queue);
	print_distances(&the_graph);
}