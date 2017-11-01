#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <assert.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};


void testcase() {
	int n, m;
	cin >> n >> m;

	//total of outgoing demand for each node
	vector<int> demand_out(n);
	//total of incoming supply for each node
	vector<int> supply_in(n);
	//total demand in locations
	int total_demand = 0;

	// Create Graph and Maps
	int source = n;
	int target = n+1;
	int V = n + 2;
	Graph G(V);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	//Build graph
	for(int i=0; i<n; i++) {
		int stationed, needed;
		cin >> stationed >> needed;

		total_demand += needed;
		demand_out[i] = needed;
		supply_in[i] = stationed;
	}


	//add paths between locations
	//The idea is to add up all outgoing demand for each location
	//which is the sum of all outgoing minimum capacities plus
	//the amount of soldiers needed in that location.
	//The same thing is done for supply plus guaranteed incoming
	//soldiers (incoming minimum capacity)
	for(int i=0; i<m; i++) {
		int from, to, c_min, c_max;
		cin >> from >> to >> c_min >> c_max;

		demand_out[from] += c_min;
		supply_in[to] += c_min;
		total_demand += c_min;

		//add only the difference between min capacity and
		//max capacity, the rest (min capacity) is handled directly between
		//location, source and target
		assert(c_max - c_min >= 0);
		eaG.addEdge(from, to, c_max - c_min);
	}


	//add supply and demand totals by connecting 
	//each location to source and target
	for(int i=0; i<n; i++) {
		eaG.addEdge(source, i, supply_in[i]);
		eaG.addEdge(i, target, demand_out[i]);
	}

	long flow = boost::push_relabel_max_flow(G, source, target);

	if(flow != total_demand) cout << "no\n";
	else cout << "yes\n";

}

int main(void) {

	//disable for performance
	ios_base::sync_with_stdio(false);
	//read total number of test cases
	int n_total_testcases;
	cin >> n_total_testcases;

	//do for each testcase
	for(int n_test = 0; n_test < n_total_testcases; n_test++) {
		testcase();
	}

	return 0;
}