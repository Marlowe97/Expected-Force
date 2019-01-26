// ExpectedForce.cpp
// Author: Marlowe Chung
// Date: 2018/01/23
// Version: 1.0
// Purpose: Calculate the expected force of infection of specific vertex in the graph

#include <math.h>
#include "igraph.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

int ForceInfection(igraph_t* graph, igraph_vs_t* cluster)
{
	igraph_vector_ptr_t result;
	igraph_vector_ptr_init(&result, 0);
	igraph_integer_t ord = 1;
	igraph_neighborhood(graph, &result, *cluster, ord, IGRAPH_ALL);
	vector<int> neighbors;
	for (int i = 0; i < igraph_vector_ptr_size(&result); i++)
	{
		igraph_vector_t* partNeigh = (igraph_vector_t*) igraph_vector_ptr_e(&result, i);
		for (int j = 0; j < igraph_vector_size(partNeigh); j++)
		{
			neighbors.push_back(VECTOR(*partNeigh)[j]);
		}
	}
	sort(neighbors.begin(), neighbors.end());
	neighbors.erase(unique(neighbors.begin(), neighbors.end()), neighbors.end());

	// Here, we know the cluster will have three elements, hence the "- 3"
	// if the cluster size is unknown, better to minus the size of the clust
	int numNeighbors = neighbors.size() - 3;
	return numNeighbors;
}

igraph_bool_t bfs_callback(const igraph_t *graph,
	igraph_integer_t vid,
	igraph_integer_t pred,
	igraph_integer_t succ,
	igraph_integer_t rank,
	igraph_integer_t dist,
	void *extra) {
	return ((long int)dist == 2);
}


/*
igraph_bool_t bfs_callback(const igraph_t *graph, igraph_integer_t vid,
	igraph_integer_t pred,
	igraph_integer_t succ,
	igraph_integer_t rank,
	igraph_integer_t dist,
	void *extra)
{
	igraph_bool_t res;
	if (dist == 3) res = 0;
	else res = 1;
	return res;
}
*/


double ExpectedForce(igraph_integer_t qnode, igraph_t* graph)
{
	igraph_vector_t order, dist;
	/* order is stored in the same order as they were visited
	However, dist is stored as the vertex id
	*/
	igraph_vector_init(&order, 0);
	igraph_vector_init(&dist, 0);


	// Get all neighbors of the querry node at distance one and two:
	int error = igraph_bfs(graph, qnode, 0, IGRAPH_ALL, 0, 0, &order, 0, 0, 0, 0,
		&dist, &bfs_callback, 0);

	// cout << "the size of neighbors: " << igraph_vector_size(&order) << endl;
	// (&order);
	// igraph_vector_print(&dist);

	// vector of nodes at distance one
	// vector of nodes at distance two
	vector<int> disoneNodes;
	vector<int> distwoNodes;

	int numCluster = 1;

	for (int i = 0; i < igraph_vector_size(&order); i++)
	{
		int temp = VECTOR(order)[i];
		if (!temp || temp < 0 ) break;
		// cout << temp << endl;
		if (VECTOR(dist)[temp] == 1)
		{
			disoneNodes.push_back(i);
			numCluster++;
		}
		else if (VECTOR(dist)[temp] == 2)
		{
			distwoNodes.push_back(i);
			numCluster++;
		}
	}

	if (numCluster < 3) return 0;
	else
	{
		int totalFI = 0;
		int numFI = 0;
		vector<double> forceInfections;

		/* The iteration is over all nodes at distance one from the source,
		within this loop we consider both all remaining d.one.nodes
		and all d.two.nodes reachable from the current d.one.node. */
		for (int i = 0; i < disoneNodes.size() - 1; i++)
		{
			// all remaining nodes at distance 1
			for (int j = i + 1; j < disoneNodes.size(); j++)
			{
				// compute cluster 
				igraph_vs_t cluster; // the vector selector to be used in the ForceInfection
				igraph_vector_t clusterData; // the data in the cluster
				igraph_vector_init(&clusterData, 3);
				VECTOR(clusterData)[0] = qnode;
				VECTOR(clusterData)[1] = i;
				VECTOR(clusterData)[2] = j;

				igraph_vs_vector(&cluster, &clusterData);

				int clustFI = ForceInfection(graph, &cluster);

				// is there an edge between nodes i and j?
				// an indicator variable to show whether two neighbors are connected
				igraph_bool_t areConnected;
				igraph_are_connected(graph, i, j, &areConnected);

				int mult; // the total number of conditions of infection after which this state will appear
				if (areConnected) mult = 4;
				else mult = 2;
				for (int count = 0; count < mult; count++)
				{
					forceInfections.push_back(clustFI);
				}
				totalFI += mult * clustFI;
				numFI += mult;
			}

			// for all the neighbors at distance two
			for (int k = 0; k < distwoNodes.size(); k++)
			{
				// is there an edge between nodes i and k?
				// an indicator variable to show whether two neighbors are connected
				igraph_bool_t areConnected;
				igraph_are_connected(graph, i, k, &areConnected);
				if (areConnected)
				{
					// compute cluster FI
					igraph_vs_t cluster; // the vector selector to be used in the ForceInfection
					igraph_vector_t clusterData; // the data in the cluster
					igraph_vector_init(&clusterData, 3);
					VECTOR(clusterData)[0] = qnode;
					VECTOR(clusterData)[1] = i;
					VECTOR(clusterData)[2] = k;

					igraph_vs_vector(&cluster, &clusterData);

					int clustFI = ForceInfection(graph, &cluster);
					forceInfections.push_back(clustFI);
					numFI += 1;
					totalFI += clustFI;
				}
			}
		}

		// calculate the measure
		double expForce = 0;
		for (int n = 0; n < forceInfections.size(); n++)
		{
			double norm = forceInfections[n] / totalFI;
			expForce -= log(norm) * norm;
		}

		return expForce;
	}
}