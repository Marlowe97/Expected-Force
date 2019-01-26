// TestExpectedForce.cpp
// Author: Marlowe Chung
// Date: 2018/01/23
// Version: 1.0
// Purpose: Test the ExpectedForce Function to calculate the expected force measure

#include "igraph.h"
#include <iostream>

using namespace std;

int main()
{
	double ExpectedForce(igraph_integer_t qnode, igraph_t* graph);

	igraph_integer_t a = 3;
	igraph_t graph;
	igraph_vector_t v;
	igraph_real_t edges[] = { 1, 2, 2, 3, 3, 4, 2, 4, 4, 5, 4, 6, 4, 7, 2, 8, 8,9 };

	igraph_vector_view(&v, edges, sizeof(edges) / sizeof(double));
	igraph_create(&graph, &v, 0, IGRAPH_UNDIRECTED);

	cout << ExpectedForce(a, &graph);

	return 0;
}