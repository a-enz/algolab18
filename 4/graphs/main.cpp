#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS,		
		no_property,								
		property<edge_weight_t, int> 				
		>											Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;	
typedef property_map<Graph, edge_weight_t>::type	WeightMap;


void testcase() {

	int V, E;
	cin >> V >> E;

	Graph G(V);
	WeightMap weightmap = get(edge_weight, G);

	for(int i=0; i<E; i++) {
		int from, to, w;
		cin >> from >> to >> w;

		Edge e; bool success;

		tie(e, success) = add_edge(from, to, G);

		weightmap[e] = w;
	}

	//compute MST and weight of it
	vector<Vertex> predmap(V);
	prim_minimum_spanning_tree(G, make_iterator_property_map(predmap.begin(), 
							get(vertex_index, G)),
							root_vertex(0));		

	int totalweight = 0;
	for (int i = 0; i < V; i++) {
		if (predmap[i] != i) {
			Edge e; bool success;
			tie(e, success) = edge(i, predmap[i], G);
			totalweight += weightmap[e];
		}
	}

	//compute shortest paths weights to all vertices
	vector<int> distmap(V);
	Vertex start=0;

	dijkstra_shortest_paths(G, start,
		distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))));	// distance map as Named Parameter
	
	int maxdist = 0;
	Vertex furthest = start;
	for (int i = 0; i < V; ++i) {
		if (distmap[i] < INT_MAX) {
			if (distmap[i] > maxdist) {
				maxdist = distmap[i];
			}
		}
	}

	cout << totalweight << " " << maxdist << endl;

}

int main(void) {

	//disable for performance
	ios::sync_with_stdio(false);
	//read total number of test cases
	int n_total_testcases;
	cin >> n_total_testcases;

	//do for each testcase
	for(int n_test = 0; n_test < n_total_testcases; n_test++) {
		testcase();
	}

	return 0;
}