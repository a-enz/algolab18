#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS,		
		no_property,								
		property<edge_weight_t, int> 				
		>											Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;
typedef graph_traits<Graph>::edge_iterator			EdgeIt;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;	
typedef property_map<Graph, edge_weight_t>::type	WeightMap;


void testcase() {

	int V, E, n_species;
	Vertex start, finish;
	cin >> V >> E >> n_species >> start >> finish;

	//initialize a seperate instance of the graph and weightmap for each species
	vector<Graph> species_graphs(n_species, Graph(V));

	vector<WeightMap> species_weightmaps(n_species);
	for(int s=0; s<n_species; s++) {
		species_weightmaps[s] = get(edge_weight, species_graphs[s]);	
	}

	//one main graph where we will aggregate all data later
	Graph G(V);
	WeightMap weightmap = get(edge_weight, G);

	for(int i=0; i<E; i++) {
		int from, to;
		cin >> from >> to;

		//construct ecge with respective weight for all species
		for(int s=0; s<n_species; s++) {
			int w;
			cin >> w;

			Edge e; bool success;

			tie(e, success) = add_edge(from, to, species_graphs[s]);

			species_weightmaps[s][e] = w;
		}

		Edge e; bool success;
		//add edge in main graph
		tie(e, success) = add_edge(from, to, G);
		weightmap[e] = INT_MAX;
	}

	//read all hive locations
	vector<Vertex> species_hive(n_species);
	for(int s=0; s<n_species; s++) {
		cin >> species_hive[s];
	}

	//compute MST for each species
	vector< vector<Vertex> > species_predmap(n_species, vector<Vertex>(V));
	for(int s=0; s<n_species; s++) {
		prim_minimum_spanning_tree(species_graphs[s], 
							make_iterator_property_map(species_predmap[s].begin(), 
							get(vertex_index, species_graphs[s])),
							root_vertex(species_hive[s]));
	}


	//now construct the weights in the main graph
	//by taking the minimum over all species edge weights
	//but only if the edge is in the MST of the species
	for(int s=0; s<n_species; s++) {
		for(int i = 0; i < species_predmap[s].size(); i++) {
			if(species_predmap[s][i] != i) {
				Edge e_s, e;
				tie(e_s, std::ignore) = edge(i, species_predmap[s][i], species_graphs[s]);
				tie(e, std::ignore) = edge(i, species_predmap[s][i], G);

				int species_w = species_weightmaps[s][e_s];
				if(species_w < weightmap[e]) {
					weightmap[e] = species_w;
				}
			}
		}
	}


	//now compute shortest paths
	vector<int> distmap(V);

	dijkstra_shortest_paths(G, start,
		distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G)))
	);

	cout << distmap[finish] << endl;

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