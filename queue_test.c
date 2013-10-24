#include "queue_test.h"
#include "test_util.h"

#include <assert.h>

Graph *graph;
Vertex *vertices;
Queue *q;

#define EQUAL(x, y, z, w) ((x) == (z) && (y) == (w)) || ((x) == (w) && (y) == (z))

Vertex *q_pop_min()
{
	return Queue__pop_min(q, &the_graph);
}

int pop_min()
{
	return q_pop_min() - &the_graph.vertices[0];
}

void insert(Vertex_Num v, Distance d)
{
	Queue__insert(q, &vertices[v], d, graph);
}

void init()
{
	Queue__init(q);

}

void assert_finished()
{
	assert(q_pop_min() == NULL);
}

void test_insert_pop_min()
{
	init();

	insert(0, 0);
	insert(1, 0);

	int v1 = pop_min();
	int v2 = pop_min();
	assert(EQUAL(v1, v2, 0, 1));

	assert_finished();
}

void test_inserting_two_vertices_with_zero_distance()
{
	init();

	insert(0, 0);
	assert(pop_min() == 0);

	assert_finished();
}

void test_inserting_vertex_with_3_dist_then_2_dist()
{
	init();

	insert(17, 3);
	insert(20, 2);

	assert(pop_min() == 20);
	assert(pop_min() == 17);

	assert_finished();
}

void test_insert_max_distance()
{
//	init();
//
//	// This is done for faster TESTING ONLY, obviously not part of the API;
//	// Do not use this technique yourself unless you know what you're doing
//	q->min_distance_candidate = DISTANCE__MAX - 10;
//	q->max_distance_ever_seen = DISTANCE__MAX - 20;
//
//	insert(34, DISTANCE__MAX);
//	assert(pop_min() == 34);
//	assert(graph->vertices[34].distance == DISTANCE__MAX);
//
//	assert_finished();
}

bool_t setup_done = FALSE;
void setup()
{
	if (setup_done) {
		return;
	}

	graph = &the_graph;
	vertices = &graph->vertices[0];
	q = the_queue;

	load_graph("example.graph", &parse_simple_space_delimited_line, graph);

	setup_done = TRUE;
}

void run_build_tests()
{
	setup();

	test_insert_pop_min();
	test_insert_pop_min();
	test_inserting_vertex_with_3_dist_then_2_dist();
}

void run_extended_tests()
{
	setup();

	test_insert_max_distance();
}

void run_all_tests()
{
	run_build_tests();
	run_extended_tests();
}
