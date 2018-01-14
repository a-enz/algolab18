#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

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
typedef	boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;


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
	int n_intersec, m_streets, k_stations, l_photos;
	cin >> n_intersec >> m_streets >> k_stations >> l_photos;

	//read police stations
	vector<int> station(k_stations);
	for (int i = 0; i < k_stations; ++i)
	{
		cin >> station[i];
	}

	//read photo locations
	vector<int> photo(l_photos);
	for (int i = 0; i < l_photos; ++i)
	{
		cin >> photo[i];
	}


	// read one-way streets
	vector<int> street_from(m_streets);
	vector<int> street_to(m_streets);
	for (int i = 0; i < m_streets; ++i)
	{
		cin >> street_from[i];
		cin >> street_to[i];
	}

	// create a graph to find out which photos are reachable
	int V = 2 * n_intersec + 2;
	Vertex source = V-2;
	Vertex target = V-1;
	Graph G(V);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	// inflow to stations
    // outflow from stations
	for (int i = 0; i < k_stations; ++i)
	{
		eaG.addEdge(source, station[i], 1);
        eaG.addEdge(n_intersec + station[i], target, 1);
	}

	// streets
	for (int i = 0; i < m_streets; ++i)
	{
		eaG.addEdge(street_from[i], street_to[i], k_stations);
        eaG.addEdge(street_from[i] + n_intersec, street_to[i] + n_intersec, 1);
	}

	// redirect photo locations to copy graph
	for (int i = 0; i < l_photos; ++i)
	{
		eaG.addEdge(photo[i], n_intersec + photo[i], 1);
	}

	//compute flow and check which photos are reached
	int flow = boost::push_relabel_max_flow(G, source, target);

	cout << flow << endl;
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