#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, directedS,
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
	int r_start, b_start;

	vector< vector<int> > adj_list(V, vector<int>());

	cin >> r_start >> b_start;

	//transform to zero indexing:
	r_start -= 1;
	b_start -= 1;

	//read transition, add the inverse edge
	for(int i=0; i<E; i++) {
		int from, to;
		cin >> from >> to;

		from -= 1;
		to -= 1;

		adj_list[from].push_back(to);
		
		Edge e;
		tie(e, std::ignore) = add_edge(to, from, G);

		weightmap[e] = 1;
	}

	std::vector<Vertex> predmap(V);
	std::vector<int> distmap(V);
	Vertex start = V-1;
	dijkstra_shortest_paths(G, start,
		predecessor_map(make_iterator_property_map(predmap.begin(), get(vertex_index, G))).
		distance_map(make_iterator_property_map(distmap.begin(), get(vertex_index, G))));


	int r_loc = r_start, b_loc = b_start;

	int round = 0;
	while(true) {
		//move red and black meeple

		if(round%4==0) {
			//sherlock moves R optimaly
			r_loc = predmap[r_loc];
		}
		else if(round%4==1) {
			//moriarty moves B optimaly
			b_loc = predmap[b_loc];
		}
		else if(round%4==2) {
			//sherlock moves B in some way bad for moriarty
			//move to highest distmap value of all nodes
			//reachable from b_loc
		
			int dist = -1, new_loc;
			for(int t : adj_list[b_loc]) {
				if(dist < distmap[t]) {
					dist = distmap[t];
					new_loc = t;
				}
			}

			b_loc = new_loc;
		}
		else {//round%4==3
			//moriarty moves R in some way bad for sherlock
			int dist = -1, new_loc;
			for(int t : adj_list[r_loc]) {
				if(dist <= distmap[t]) {
					dist = distmap[t];
					new_loc = t;
				}
			}

			r_loc = new_loc;
		}

		round++;

		if(r_loc==V-1) {
			cout << "0\n";
			break;
		}
		else if(b_loc==V-1) {
			cout << "1\n";
			break;
		}
	}


	/*
	cout << "distmap: ";
	for(int i=0; i<V; i++) {
		cout << distmap[i] << " ";
	}
	cout << endl;
	*/
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