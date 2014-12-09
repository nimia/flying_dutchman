/*
  Copyright (C) 2003, Guillaume-Latapy - LIAFA (University of Paris 7)

  Disclaimer: this code is provided 'as-is', without any express or
  implied warranty. In no event will the authors be held liable for
  any damages arising from the use of this software.

  Permission is granted to anyone to use this code for any purpose,
  to alter it and redistribute it without restrictions. Just feel
  free to inform us if you plan to use it. If you face any problems
  or find any bug, please contact us using our webpage:
    http://www.liafa.jussieu.fr/~guillaume/

  --------------------------------------------------

  Generates a random Albert-Barabasi graph. The core of the network is 
  a random Erdos-Renyi graph, and new vertices are attached using 
  linear preferential attachment. The number of links per new vertex 
  can be tuned.
  Generated graph is sent to stdout as a set of links (one per line).
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <fstream> 
#include <vector>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

using namespace std;

void usage(char *program_name, int status) {
  if (status == EXIT_SUCCESS)
    {
      cout << "Usage: " << program_name << " -n nbvertices -m nblinks [-c core_size] [-p core_connectivity]" << endl
           << "  Generates a random Albert-Barabasi graph using preferential attachment. The core" << endl
           << "  of the graph is an Erdos-Renyi graph and each new vertex draw some links to older vertices." << endl
           << "  Graph is printed on stdout with one link per line." << endl
           << "    -n: number of vertices of the random graph (non negative integer)" << endl
           << "    -m: number of links for each new vertex (non negative real)" << endl
           << "    -c: size of the core graph (non negative integer, 10 by default)" << endl
           << "    -p: connection probability of the core graph (non negative real, .1 by default)" << endl
           << "    -d: edge distance will be uniformly chosen from 1 to d" << endl
           << "    -h, this usage" << endl
           << "Remarks:" << endl
           << "  If the number of links is non integer (2.3 for instance), then each new vertex has this " << endl
	   << "  degree on average (for instance, 70% of the vertices have a degree of 2, 30% of 3). This " << endl
	   << "  scheme allows the creation of graphs of any size." << endl
	   << "  The couterpart of the previous remark is that the total number of links is not defined." << endl;
    }
  else
    {
      cerr << "Try '" << program_name << " -h' for usage information." << endl;
    }
  exit(status);
}

void
parse_args(int argc, char **argv, int &n, float &m, int &c, float &p, int &d) {
  extern char *optarg;
  extern int optind, opterr, optopt;
  char copt;

  c=10;
  p=0.1;

  while ((copt = getopt(argc, argv, "n:m:d:c:p:h")) != EOF){
    switch (copt) {
    case 'h':       /* help */
      usage(argv[0], EXIT_SUCCESS);
      break;
    case 'n':       /* nb vertices */
      n=atoi(optarg);
      break;
    case 'm':       /* nb links */
      m=atof(optarg);
      break;
    case 'c':       /* core size */
      c=atoi(optarg);
      break;
    case 'p':       /* connection probability */
      p=atof(optarg);
      break;
    case 'd':       /* upper limit for distance distribution */
      d=atoi(optarg);
      break;

    case ':':       /* missing operand */
      cerr << argv[0] << ": Option -" << optopt << " requires an operand." << endl;
      usage(argv[0], EXIT_FAILURE);
      break;
    case '?':       /* unknown option */
      cerr << argv[0] << ": Unrecognized option -" << optopt << endl;
      usage(argv[0], EXIT_FAILURE);
    }
  }
}

int
main(int argc, char **argv) {
  srand(time(NULL));
  
  int n=-1, c, d;
  float m=-1., p;
  
  parse_args(argc, argv, n, m, c, p, d);

  if (n<0 || m<0)
    usage(argv[0], EXIT_SUCCESS);
  
  vector<int> degree_cumul;
  int degree_cumul_size=0;

  for(int i=0 ; i<c ; i++)
    for(int j=i+1 ; j<c ;j++)
      if ((random()*1.)/(RAND_MAX*1.)<=p) {
	degree_cumul.push_back(i);
	degree_cumul.push_back(j);
	degree_cumul_size+=2;
	int distance = (random() % d) + 1;
	cout << i << " " << j << " " << distance << endl;
      }

  for(int i=c ; i<n ; i++) {
    double var = m - floor(m);
    double plusone = (random()*1.)/(RAND_MAX*1.);
    double toplimit=(plusone<var)?1:0;

    for(int j=1 ; j<=m+toplimit ; j++) {
      int dest = degree_cumul[random()%degree_cumul_size];
      degree_cumul.push_back(dest);
      degree_cumul.push_back(i);
      int distance = (random() % d) + 1;
      cout << i << " " << dest << " " << distance << endl;
    }
    degree_cumul_size+=2*(int)floor(m);
  }
}
