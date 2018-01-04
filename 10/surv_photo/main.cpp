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
	int V = m_streets + 2;
	Vertex source = V-2;
	Vertex target = V-1;
	Graph G_1(V);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G_1);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G_1);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G_1);
	EdgeAdder eaG_1(G_1, capacitymap, revedgemap);

	// inflow to stations
	for (int i = 0; i < k_stations; ++i)
	{
		eaG_1.addEdge(source, station[i], 1);
	}

	// streets
	for (int i = 0; i < m_streets; ++i)
	{
		eaG_1.addEdge(street_from[i], street_to[i], k_stations);
	}

	// outflow
	for (int i = 0; i < l_photos; ++i)
	{
		eaG_1.addEdge(photo[i], target, 1);
	}

	//compute flow and check which photos are reached
	int flow_1 = boost::push_relabel_max_flow(G_1, source, target);

	vector<int> reached_photo;
	OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(target,G_1), G_1); e != eend; ++e) {
    	int edge_flow = rescapacitymap[*e] - capacitymap[*e];
    	if(edge_flow != 0) {
	    	int p = boost::target(*e, G_1);
	    	reached_photo.push_back(p);

	    	//cout << "reached " << p << endl;
    	}
    }

    // do the same flow problem in reverse starting from the reached photo vertices
	Graph G_2(V);
	capacitymap = boost::get(boost::edge_capacity, G_2);
	revedgemap = boost::get(boost::edge_reverse, G_2);
	rescapacitymap = boost::get(boost::edge_residual_capacity, G_2);
	EdgeAdder eaG_2(G_2, capacitymap, revedgemap);

	// inflow to reached photos
	for (int i = 0; i < reached_photo.size(); ++i)
	{
		eaG_2.addEdge(source, reached_photo[i], 1);
	}

	// streets
	for (int i = 0; i < m_streets; ++i)
	{
		eaG_2.addEdge(street_from[i], street_to[i], 1);
	}

	// outflow
	for (int i = 0; i < k_stations; ++i)
	{
		eaG_2.addEdge(station[i], target, 1);
	}

	//compute flow and check which photos are reached
	int flow_2 = boost::push_relabel_max_flow(G_2, source, target);

	cout << flow_2 << endl;
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