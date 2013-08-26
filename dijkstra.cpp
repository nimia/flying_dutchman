#include <time.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include "list.h"
#include "huge_header.h"
#include "queue.h"

bool_t parse_line(char *line, char delimiter, int *first, int *second)
{
	if (strcmp(line, "\r\n") == 0) {
		return FALSE;
	}
	char *delimiter_in_line = strchr(line, delimiter);
	*delimiter_in_line = '\0';
	*first = atoi(line);
	*second = atoi(delimiter_in_line + 1);
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
		int first, second;
		if (parse_line(line, delimiter, &first, &second)) {
			if (first >= NUM_OF_VERTICES || second >= NUM_OF_VERTICES) {
				printf("Encountered vertex with too big an index - did you remember to set NUM_OF_VERTICES?\n");
				abort();
			}
			Node *first_node = &graph->nodes[first];
			Node *second_node = &graph->nodes[second];
			first_node->neighbors.push_front({second_node, 1});
		}
	}
	fclose(f);
}

void dijkstra(Graph *graph, Node *source)
{
	Queue q;
	Queue__init(&q);

	Queue__insert(&q, source, 0, graph);

	Node *u = Queue__pop_min(&q, graph);
	while (u) {
		for (list<Node_And_Distance>::iterator i = u->neighbors.begin(); i != u->neighbors.end(); ++i) {
			Node *v = (*i).node;
			Distance alt = u->distance + (*i).distance;

			if (alt < v->distance) {
				v->distance = alt;
				Queue__decrease_key(&q, v, v->distance, graph);
			}
		}

		u = Queue__pop_min(&q, graph);
	}
}

Graph the_graph;
int main()
{
	return 1;
}
