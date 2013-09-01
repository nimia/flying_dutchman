#include "queue_test.h"
#include "test_util.h"

#include <assert.h>

Graph *graph = &the_graph;
Vertex *vertexs = the_graph.vertexs;
Queue *q = &the_queue;

#define EQUAL(x, y, z, w) ((x) == (z) && (y) == (w)) || ((x) == (w) && (y) == (z))

int pop_min()
{
	return Queue__pop_min(&the_queue, &the_graph) - &the_graph.vertexs[0];
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
	load_graph("example", ' ', graph);

	test_insert_pop_min();
	test_insert_pop_min();
	test_inserting_vertex_with_3_dist_then_2_dist();
}
