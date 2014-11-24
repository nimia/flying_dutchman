// Copyright 2004 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
// humble changes by Nimrod Aviram
#define BOOST_GRAPH_DIJKSTRA_TESTING

#define __STDC_LIMIT_MACROS
#include <stdint.h>

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/dijkstra_shortest_paths_no_color_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/timer.hpp>
#include <vector>
#include <iostream>

#include <iostream>
#include <fstream>

#include <iterator>
#include <utility>
#include <boost/random/uniform_int.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/detail/lightweight_test.hpp>

#include "parse.h"
#include "utils.h"

using namespace boost;

typedef adjacency_list<vecS, vecS, directedS, no_property,
                       property<edge_weight_t, int32_t> > Graph;

#define BOOST__NUM_OF_VERTICES 23947348

void load_graph(char *filename, line_parse_func_t *parse_line, Graph &g, bool_t bidirectional_edges)
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
			if (bidirectional_edges) {
				add_edge(vertex(second, g), vertex(first, g), distance, g);
			}
			DEBUG("%d => %d, distance %d\n", first, second, distance);
		}
	}

	fclose(f);
}

std::ofstream output_file;

void print_distances(std::vector<int32_t> &distances)
{
	for (int i = 0; i < distances.size(); i++) {
		if (distances[i] != INT32_MAX) {
			output_file << i << " " << distances[i] << std::endl;
		}
	}
}

double min(double a, double b)
{
	if (a > b) {
		return b;
	}
	return a;
}

int main(int argc, char* argv[])
{
  Graph g(BOOST__NUM_OF_VERTICES);

  char *graph_path = argv[2];

  bool_t summary = FALSE;
  if (!strcmp(argv[1], "usa")) {
	  load_graph(graph_path, &parse_usa_challenge_line, g, FALSE);
  } else if (!strcmp(argv[1], "summary")) {
	  summary = TRUE;
	  load_graph(graph_path, &parse_usa_challenge_line, g, FALSE);
  } else if (!strcmp(argv[1], "er")) {
	  load_graph(graph_path, &parse_boost_line, g, FALSE);
  } else if (!strcmp(argv[1], "p2p")) {
	  load_graph(graph_path, &parse_p2p_line, g, TRUE);
  } else {
	  printf("Not sure what you want, check your arguments\n");
	  exit(1);
  }

  int starting_vertex = atoi(argv[3]);
  if (summary) {
	  printf("%d", starting_vertex);
  }
  std::vector<int32_t> relaxed_heap_distances(BOOST__NUM_OF_VERTICES);
  std::vector<int32_t> no_color_map_distances(BOOST__NUM_OF_VERTICES);

  std::string graph_basename = my_basename(graph_path);
  if (!summary) {
	  printf("Basename is %s\n", graph_basename.c_str());
	  std::string output_file_path = "/localwork/dijkstra/boost_results_on_";
	  output_file_path += graph_basename;
	  output_file_path += "_";
	  output_file_path += lexical_cast<std::string>(starting_vertex);
	  printf("Output file is %s\n", output_file_path.c_str());
	  output_file.open(output_file_path.c_str());
	  output_file << "Starting Boost Dijkstra on graph " << graph_basename << ", from starting vertex " << starting_vertex << std::endl;
  }

  timer t;
  dijkstra_relaxed_heap = true;
  t.restart();
  dijkstra_shortest_paths(g, vertex(starting_vertex, g),
                          distance_map(&relaxed_heap_distances[0]));
  double dijkstra_time = t.elapsed();

  dijkstra_relaxed_heap = false;
  t.restart();
  dijkstra_shortest_paths(g, vertex(starting_vertex, g),
                          distance_map(&relaxed_heap_distances[0]));
  dijkstra_time = min(dijkstra_time, t.elapsed());

  dijkstra_relaxed_heap = true;
  t.restart();
  dijkstra_shortest_paths_no_color_map
    (g, vertex(starting_vertex, g),
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
  dijkstra_time = min(dijkstra_time, t.elapsed());

  dijkstra_relaxed_heap = false;
  t.restart();
  dijkstra_shortest_paths_no_color_map
    (g, vertex(starting_vertex, g),
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
  dijkstra_time = min(dijkstra_time, t.elapsed());

  if (summary) {
	std::cout << " " << dijkstra_time << std::endl;
	exit(0);
  } else {
	  output_file << "Algorithm took " << dijkstra_time << " seconds." << std::endl;
  }

  print_distances(relaxed_heap_distances);

  return 0;
}
