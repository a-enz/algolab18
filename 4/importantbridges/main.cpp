#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/biconnected_components.hpp>

using namespace std;
using namespace boost;


typedef adjacency_list<vecS, vecS, undirectedS,		
		no_property,
		property<edge_weight_t, int>				
		>											Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;
typedef graph_traits<Graph>::edge_iterator			EdgeIt;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;	
typedef property_map<Graph, edge_weight_t>::type ComponentMap;


void testcase() {

	int V, E;
	cin >> V >> E;

	Graph G(V);

	for(int i=0; i<E; i++) {
		int from, to;
		cin >> from >> to;

		Edge e; bool success;

		tie(e, success) = add_edge(from, to, G);
		//cout << "from " << from << " to " << to << endl;
	}

  
    ComponentMap componentMap = get(edge_weight, G);

    int num_comp = biconnected_components(G, componentMap);

    //cout << "num_comp: " << num_comp << endl;
    //map: component -> number of edges in that component
    std::unordered_map<int, int> edge_counts(num_comp);



    //count how many edges are in each component
    //those components containing only one edge are the ones we want

    EdgeIt eIt, eIt_end;
    for(boost::tie(eIt, eIt_end)=edges(G); eIt != eIt_end; eIt++) {
    	bool success;
    	std::tie(std::ignore, success) = 
    		edge_counts.insert(make_pair(componentMap[*eIt], 1));

    		if(not success) edge_counts[componentMap[*eIt]]++; 
    }

    //iterate over edges and filter out those with edge_count = 1
    //add edge source and target (n_i, n_j) s.t. n_i < n_j
    vector< pair<int, int> > bridges;
    for(boost::tie(eIt, eIt_end)=edges(G); eIt != eIt_end; eIt++) {
    	if(edge_counts[componentMap[*eIt]] == 1){
    		Vertex source = boost::source(*eIt, G);
    		Vertex target = boost::target(*eIt, G);

    		//Switch target and source
    		if(source < target) bridges.push_back(make_pair(source, target));
    		else bridges.push_back(make_pair(target, source));
    	}
    }

	cout << bridges.size() << endl;


	//sort bridges lexicographically
	std::sort(bridges.begin(), bridges.end(),
		[&](const pair<int,int>& a, const pair<int,int>& b)-> bool {
			if(a.first==b.first) {
				return a.second < b.second;
			}
			else {
				return a.first < b.first;
			}
		}
	);

	//print bridges
	for(int i = 0; i<bridges.size(); i++ ) {
		cout << bridges[i].first << " " << bridges[i].second << endl;
	}
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