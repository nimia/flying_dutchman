#include "bellman_ford.h"
#include "utils.h"
#include "list.h"

void bellman_ford(Graph *graph, Vertex_Num source, __attribute__((unused)) Queue *dummy)
{
	graph->vertices[source].distance = 0;

	while (1) {
		bool_t relaxed_this_iteration = FALSE;

		for (int i = 0; i <= graph->max_vertex_num; i++) {
			Vertex *u = &graph->vertices[i];

			Edge *e;
			Vertex__FOR_EACH_EDGE(u, e, graph) {
				Vertex *v = Edge__DEST(e, graph);

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
