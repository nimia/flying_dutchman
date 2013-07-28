#include <time.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "global.h"
#include <unistd.h>
#include <list>
#include "BinaryHeap/BinaryHeap.h"

using namespace std;

struct Graph {
	list<Vertex_And_Distance> _neighbors[NUM_OF_VERTICES];
};

bool parse_line(char *line, char delimiter, int *first, int *second)
{
	if (strcmp(line, "\r\n") == 0) {
		return false;
	}
	char *delimiter_in_line = strchr(line, delimiter);
	*delimiter_in_line = '\0';
	*first = atoi(line);
	*second = atoi(delimiter_in_line + 1);
	return true;
}

void load_graph(char *filename, char delimiter, Graph &graph)
{
	FILE *f = fopen(filename, "r");
	char line[128];

	while (fgets(line, sizeof line, f) != NULL ) {
		int first, second;
		if (parse_line(line, delimiter, &first, &second)) {
			if (first >= NUM_OF_VERTICES || second >= NUM_OF_VERTICES) {
				printf("Encountered vertex with too big an index - did you remember to set NUM_OF_VERTICES?\n");
				abort();
			}
			graph._neighbors[first].push_front({(Vertex)second, 1});
		}
	}
	fclose(f);
}

void dijkstra(Graph &graph, Vertex source, Distance distances[NUM_OF_VERTICES])
{
	Queue_Element_Pointer queue_elements[NUM_OF_VERTICES];
	Queue q;
	Queue__init(&q, queue_elements);

	for (int i = 0; i < NUM_OF_VERTICES; i++) {
		distances[i] = DISTANCE_INFINITY;
	}

	Queue__insert(&q, source, 0);
	distances[source] = 0;

	while (!Queue__empty(&q)) {
		Vertex u = Queue__pop_min(&q);
		if (distances[u] == DISTANCE_INFINITY) {
			return;
		}
		for (list<Vertex_And_Distance>::iterator i = graph._neighbors[u].begin(); i != graph._neighbors[u].end(); ++i) {
			Vertex v = (*i)._vertex;
			Distance alt = distances[u] + (*i)._distance;
			if (alt < distances[v]) {
				distances[v] = alt;
				Queue__decrease_key(&q, queue_elements[v], distances[v]);
			}
		}
	}
}

Graph the_graph;
int main()
{
	return 1;
}
