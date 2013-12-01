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
#include <fstream>

#include <iterator>
#include <utility>
#include <boost/random/uniform_int.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/detail/lightweight_test.hpp>

using namespace boost;

double min(double a, double b)
{
	if (a > b) {
		return b;
	}
	return a;
}

int main(int argc, char* argv[])
{
	if (argc != 4) {
		std::cout << "Usage: er_generator num_of_vertices num_of_edges seed" << std::endl;
		exit(1);
	}

  unsigned n = lexical_cast<int>(argv[1]);
  unsigned m = lexical_cast<int>(argv[2]);
  int seed = lexical_cast<int>(argv[3]);

  std::string output_file_path = "/localwork/er_";
  output_file_path += lexical_cast<std::string>(n);
  output_file_path += "_";

  output_file_path += lexical_cast<std::string>(m);
  output_file_path += "_";

  output_file_path += lexical_cast<std::string>(seed);

  std::cout << "Output file is " << output_file_path.c_str() << std::endl;
  std::ofstream output_file;
  output_file.open(output_file_path.c_str());

  // Build random graph
  typedef adjacency_list<vecS, vecS, directedS, no_property,
                         property<edge_weight_t, uint32_t> > Graph;
  minstd_rand gen(seed);
  double p = double(m)/(double(n)*double(n));
  Graph g(erdos_renyi_iterator<minstd_rand, Graph>(gen, n, p),
          erdos_renyi_iterator<minstd_rand, Graph>(),
          n);

  uniform_real<double> rand01(0.0, 1.0);
  graph_traits<Graph>::edge_iterator ei, ei_end;
  for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
	uint32_t weight = int(rand01(gen) * 10000);
    put(edge_weight, g, *ei, weight);
    output_file << *ei << ' ' << weight << std::endl;
  }

  std::vector<double> relaxed_heap_distances(n);
  std::vector<double> no_color_map_distances(n);

  // Run relaxed heap version
  timer t;
  dijkstra_relaxed_heap = true;
  t.restart();
  dijkstra_shortest_paths(g, vertex(0, g),
                          distance_map(&relaxed_heap_distances[0]));
  double dijkstra_time = t.elapsed();

  dijkstra_relaxed_heap = false;
  t.restart();
  dijkstra_shortest_paths(g, vertex(0, g),
                          distance_map(&relaxed_heap_distances[0]));
  dijkstra_time = min(dijkstra_time, t.elapsed());

  // Run Michael's no-color-map version
  dijkstra_relaxed_heap = true;
  t.restart();
  dijkstra_shortest_paths_no_color_map
    (g, vertex(0, g),
     boost::dummy_property_map(),
     boost::make_iterator_property_map(&no_color_map_distances[0],
                                       get(boost::vertex_index, g),
                                       0.),
     get(boost::edge_weight, g),
     get(boost::vertex_index, g),
     std::less<double>(),
     boost::closed_plus<double>(),
     (std::numeric_limits<double>::max)(),
     0,
     make_dijkstra_visitor(null_visitor())
     );
  dijkstra_time = min(dijkstra_time, t.elapsed());

  dijkstra_relaxed_heap = false;
  t.restart();
  dijkstra_shortest_paths_no_color_map
    (g, vertex(0, g),
     boost::dummy_property_map(),
     boost::make_iterator_property_map(&no_color_map_distances[0],
                                       get(boost::vertex_index, g),
                                       0.),
     get(boost::edge_weight, g),
     get(boost::vertex_index, g),
     std::less<double>(),
     boost::closed_plus<double>(),
     (std::numeric_limits<double>::max)(),
     0,
     make_dijkstra_visitor(null_visitor())
     );
  dijkstra_time = min(dijkstra_time, t.elapsed());

  output_file << "Algorithm took " << dijkstra_time << " seconds." << std::endl;

  return 0;
}
