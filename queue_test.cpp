#include "queue_test.h"
#include "test_util.h"

#include <assert.h>

Graph *graph;
Vertex *vertexs;
Queue *q;

#define EQUAL(x, y, z, w) ((x) == (z) && (y) == (w)) || ((x) == (w) && (y) == (z))

int pop_min()
{
	return Queue__pop_min(q, &the_graph) - &the_graph.vertexs[0];
}

void insert(Vertex_Num v, Distance d)
{
	Queue__insert(q, &vertexs[v], d, graph);
}

void test_insert_pop_min()
{
	Queue__init(q);

	insert(0, 0);
	insert(1, 0);

	int v1 = pop_min();
	int v2 = pop_min();
	assert(EQUAL(v1, v2, 0, 1));
}

void test_inserting_two_vertexs_with_zero_distance()
{
	Queue__init(q);

	insert(0, 0);
	assert(pop_min() == 0);
}

void test_inserting_vertex_with_3_dist_then_2_dist()
{
	Queue__init(q);

	insert(17, 3);
	insert(20, 2);

	assert(pop_min() == 20);
	assert(pop_min() == 17);
}

void run_all_tests()
{
	graph = &the_graph;
	vertexs = &graph->vertexs[0];
	q = the_queue;

	load_graph("example", ' ', graph, 0);

	test_insert_pop_min();
	test_insert_pop_min();
	test_inserting_vertex_with_3_dist_then_2_dist();
}
