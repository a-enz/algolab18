#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, directedS,		
		no_property,								
		property<edge_weight_t, int> 				
		>											Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;	
typedef property_map<Graph, edge_weight_t>::type	WeightMap;

void output_distance(int from, int to, int dist) {
	if(from == to) cout << "0\n";
	else if(dist==INT_MAX) cout << "unreachable\n";
	else cout << dist << endl;

}

void testcase() {

	int V, E, n_queries;
	cin >> V >> E >> n_queries;

	Graph G(V);
	WeightMap weightmap = get(edge_weight, G);

	//lazy way to skip the vertex location input
	for(int i=0; i<V; i++) {
		int longitude, latitude;
		cin >> longitude >> latitude;
	}

	//read and insert graph edges
	for(int i=0; i<E; i++) {
		int from, to, w;
		cin >> from >> to >> w;

		Edge e; bool success;

		tie(e, success) = add_edge(from, to, G);

		weightmap[e] = w;
	}

	//read queries
	vector< pair<int, int> > query_from_to(n_queries);
	for(int i=0; i<n_queries; i++) {
		int from, to;
		cin >> from >> to;
		query_from_to[i] = make_pair(from, to);
	}

	/* we need to remember sorted order so we can output it correctly
	//sort queries according to start, so we can query several times
	//for the same start vertex
	std::sort(query_from_to.begin(), query_from_to.end(),
			[&](const pair<int, int>& a, const pair<int, int>& b) -> bool{
				return a.first < b.first;
			}
		);
	*/

	if(n_queries==0) return;
	//compute shortest paths for all queries, only recompute dijkstra if
	//starting vertex changes
	vector<int> distmap(V);
	int start_vertex=query_from_to[0].first;
	
	dijkstra_shortest_paths(G, start_vertex,
		distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))));
	

	output_distance(start_vertex,
					query_from_to[0].second,
					distmap[query_from_to[0].second]);


	for(int i=1; i<n_queries; i++) {
		int distance;
		if(query_from_to[i].first != start_vertex) {
			//recompute with new start vertex
			start_vertex = query_from_to[i].first;

			dijkstra_shortest_paths(G, start_vertex,
				distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))));
		}

		output_distance(start_vertex,
				query_from_to[i].second,
				distmap[query_from_to[i].second]);
	}
}

int main(void) {

	//disable for performance
	ios::sync_with_stdio(false);
	//read total number of test cases

	testcase();

	return 0;
}