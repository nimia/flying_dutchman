// Copyright 2004 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
// humble changes by Nimrod Aviram
#define BOOST_GRAPH_DIJKSTRA_TESTING

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/dijkstra_shortest_paths_no_color_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/timer.hpp>
#include <vector>
#include <iostream>

#include <iterator>
#include <utility>
#include <boost/random/uniform_int.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/detail/lightweight_test.hpp>

#include "parse.h"

using namespace boost;

typedef adjacency_list<vecS, vecS, directedS, no_property,
                       property<edge_weight_t, int32_t> > Graph;

#define BOOST__NUM_OF_VERTICES 23947348

void load_graph(char *filename, line_parse_func_t *parse_line, Graph &g)
{
	FILE *f = fopen(filename, "r");
	char line[128];

	while (fgets(line, sizeof line, f) != NULL ) {
		Vertex_Num first, second;
		Distance distance;

		if ((*parse_line)(line, &first, &second, &distance)) {
			if (MAX(first, second) >= BOOST__NUM_OF_VERTICES) {
				printf("Encountered vertex with too big an index - did you remember to set BOOST__NUM_OF_VERTICES?\n");
				abort();
			}

			add_edge(vertex(first, g), vertex(second, g), distance, g);
			DEBUG("%d => %d, distance %d\n", first, second, distance);
		}
	}

	fclose(f);
}

void print_distances(std::vector<int32_t> &distances)
{
	for (int i = 1; i <= BOOST__NUM_OF_VERTICES; i++) {
		std::cout << i << " " << distances[i] << std::endl;
	}
}

int main(int argc, char* argv[])
{
  Graph g(BOOST__NUM_OF_VERTICES);

  load_graph(argv[1], &parse_usa_challenge_line, g);

  std::vector<int32_t> relaxed_heap_distances(BOOST__NUM_OF_VERTICES);
  std::vector<int32_t> no_color_map_distances(BOOST__NUM_OF_VERTICES);

  // Run relaxed heap version
  std::cout << "Running Dijkstra's with d-ary heap (d=4)...";
  std::cout.flush();
  timer t;
  t.restart();
  dijkstra_relaxed_heap = true;
  dijkstra_shortest_paths(g, vertex(atoi(argv[2]), g),
                          distance_map(&relaxed_heap_distances[0]));
  double relaxed_heap_time = t.elapsed();
  std::cout << relaxed_heap_time << " seconds.\n";

  print_distances(relaxed_heap_distances);


  // Run Michael's no-color-map version
  std::cout << "Running Dijkstra's (no color map) with d-ary heap (d=4)...";
  std::cout.flush();
  t.restart();
  dijkstra_relaxed_heap = true;
  dijkstra_shortest_paths_no_color_map
    (g, vertex(atoi(argv[2]), g),
     boost::dummy_property_map(),
     boost::make_iterator_property_map(&no_color_map_distances[0],
                                       get(boost::vertex_index, g),
                                       0),
     get(boost::edge_weight, g),
     get(boost::vertex_index, g),
     std::less<int32_t>(),
     boost::closed_plus<int32_t>(),
     (std::numeric_limits<int32_t>::max)(),
     0,
     make_dijkstra_visitor(null_visitor())
     );
  double no_color_map_time = t.elapsed();
  std::cout << no_color_map_time << " seconds.\n";

  return boost::report_errors();
}
