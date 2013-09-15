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

using namespace boost;

int main(int argc, char* argv[])
{
  unsigned n = 23 * 1000 * 1000;
  unsigned m = 58 * 1000 * 1000;
  int seed = (argc > 3 ? lexical_cast<int>(argv[3]) : 1);

  // Build random graph
  typedef adjacency_list<vecS, vecS, directedS, no_property,
                         property<edge_weight_t, uint32_t> > Graph;
  Graph g(n);

  add_edge(vertex(0, g), vertex(1, g), 1, g);
  add_edge(vertex(1, g), vertex(3, g), 5, g);

  std::vector<double> relaxed_heap_distances(n);
  std::vector<double> no_color_map_distances(n);

  // Run relaxed heap version
  std::cout << "Running Dijkstra's with d-ary heap (d=4)...";
  std::cout.flush();
  timer t;
  t.restart();
  dijkstra_relaxed_heap = true;
  dijkstra_shortest_paths(g, vertex(0, g),
                          distance_map(&relaxed_heap_distances[0]));
  double relaxed_heap_time = t.elapsed();
  std::cout << relaxed_heap_time << " seconds.\n";
  std::cout << "Vertex 1 distance is " << relaxed_heap_distances[1] << std::endl;
  std::cout << "Vertex 2 distance is " << relaxed_heap_distances[2] << std::endl;
  std::cout << "Vertex 3 distance is " << relaxed_heap_distances[3] << std::endl;

  // Run Michael's no-color-map version
  std::cout << "Running Dijkstra's (no color map) with d-ary heap (d=4)...";
  std::cout.flush();
  t.restart();
  dijkstra_relaxed_heap = true;
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
  double no_color_map_time = t.elapsed();
  std::cout << no_color_map_time << " seconds.\n";

  return boost::report_errors();
}
