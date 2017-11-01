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
	int width, n;
	cin >> width >> n;

	//read in brick start end
	vector< pair<int, int> > bricks(n);
	for(int i=0; i<n; i++) {
		int start, end;
		cin >> start >> end;
		if(start > end) {
			//swap
			int tmp;
			tmp = start;
			start = end;
			end = tmp;
		}

		bricks[i] = make_pair(start, end);
	}

	// Create Graph and Maps
	//Idea: every brick is a vertice v_b and every possible brick start/end
	//location is a vertice v_l. The locations vertices for 0 and 'width'
	//are special and are the source and target of the graph we're building
	//For every other location vertice v_l (except source and target)
	//we have two versions v_l and v_l' which we connect by one single
	//connection of capacity 1.
	//All vertices v_b we connect as follows:
	// - connec to location node v_start with cap 1
	// - connect to location node v_end with cap 1
	//Tus we need nodes in G:
	//for bricks: numbered 0..n-1
	//for source: number n
	//for location and copy: numbered n+1 ... (n+2*width-2)
	//for target n + 2*width-1
	int V = n+2*width;
	int source = n;
	int width_vertices_start = n;
	int target = width_vertices_start + 2*width - 1;
	Graph G(V);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);


	//Build graph
	//Connect location vertices with their copies
	//except source and target location
	for(int loc=1; loc<2*width-2; loc+=2) {
		eaG.addEdge(width_vertices_start+loc, 
					width_vertices_start+loc+1, 
					1);
	}

	for(int v=0; v<n; v++) {
		int start = bricks[v].first;
		int end = bricks[v].second;

		if(start == end) continue; //don't want to deal with this shit

		if(start==0 && end==width) {
			eaG.addEdge(source, target, 1);
		}
		else if(start==0) {
			eaG.addEdge(source, v, 1);
			eaG.addEdge(v, width_vertices_start+2*end-1, 1);
		}
		else if(end==width) {
			eaG.addEdge(width_vertices_start+2*start, v, 1);
			eaG.addEdge(v, target, 1);
		}
		else {
			eaG.addEdge(width_vertices_start+2*start, v, 1);
			eaG.addEdge(v, width_vertices_start+2*end-1, 1);
		}
	}

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