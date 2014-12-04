#include "dijkstra.h"

void dijkstra(Graph *graph, Vertex_Num source, Queue *q)
{
	Queue__init(q);

	Queue__insert(q, &graph->vertices[source], 0, graph);

	Vertex *u = Queue__pop_min(q, graph);
	while (u) {
		Edge *e;
		Vertex__FOR_EACH_EDGE(u, e, graph) {
			Vertex *v = Edge__DEST(e, graph);
			uint64_t new_distance = u->distance + e->distance;

			if (new_distance >= DISTANCE__MAX) {
				printf("Encountered too great of a distance: %lu, for vertex %d\n", new_distance, v->vertex_num);
				abort();
			}

			if (new_distance < v->distance) {
				Queue__decrease_key(q, v, new_distance, graph);
			}
		}

		u = Queue__pop_min(q, graph);
	}
}
