#include "bellman_ford.h"
#include "utils.h"

void bellman_ford(Graph *graph, Vertex_Num source)
{
	graph->vertices[source].distance = 0;

	while (1) {
		bool_t relaxed_this_iteration = FALSE;

		for (int i = 0; i <= graph->max_vertex_num; i++) {
			Vertex *u = &graph->vertices[i];

			for (list<Edge>::iterator e = u->edges.begin(); e != u->edges.end(); ++e) {
				Vertex *v = e->dest;

				if (u->distance + e->distance < v->distance) {
					v->distance = u->distance + e->distance;
					relaxed_this_iteration = TRUE;
				}
			}
		}

		if (!relaxed_this_iteration) {
			return;
		}
	}
}
