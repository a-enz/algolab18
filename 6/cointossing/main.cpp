#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
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
	int n_players, m_rounds;
	cin >> n_players >> m_rounds;

	// Create Graph and Maps
	int rounds_begin = 0;
	int player_begin = m_rounds;
	int source = m_rounds + n_players;
	int target = m_rounds + n_players + 1;
	int V = m_rounds + n_players + 2;
	Graph G(V);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	//Build graph
	//connect round to source and participating players
	for(int i=0; i<m_rounds; i++) {
		int p1, p2, outcome;
		cin >> p1 >> p2 >> outcome;

		eaG.addEdge(source, i, 1);

		if(outcome == 0) {
			//did not write down the winner
			//connect both players
			eaG.addEdge(i, player_begin+p1, 1);
			eaG.addEdge(i, player_begin+p2, 1);
		}
		else if(outcome == 1) {
			eaG.addEdge(i, player_begin+p1, 1);
		}
		else if(outcome == 2) {
			eaG.addEdge(i, player_begin+p2, 1);
		} 
		else {
			cerr << "unkown outcome in round " << i << endl;
			exit(1);
		}
	}


	int score_total=0;
	//connect players to target
	for(int i=0; i<n_players; i++) {
		int score;
		cin >> score;
		score_total += score;
		eaG.addEdge(player_begin+i, target, score);
	}

	int flow = boost::push_relabel_max_flow(G, source, target);

	if(flow != m_rounds || score_total != m_rounds) cout << "no\n";
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