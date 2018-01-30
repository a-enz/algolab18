#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Namespaces
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
typedef	boost::graph_traits<Graph>::out_edge_iterator			OutEdgeIt;


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

//global variables
const int INF_CAP = 500001;

// Functions
// ========= 
void testcases() {
    int z_zones, j_jobs;
    cin >> z_zones >> j_jobs;
    
    vector<int> cost(z_zones);
    for (unsigned int i = 0; i < z_zones; i += 1)
    {
        cin >> cost[i];
    }
    
    vector<int> profit(j_jobs);
    int total_profit = 0;
    for (unsigned int i = 0; i < j_jobs; i += 1)
    {
        cin >> profit[i];
        total_profit += profit[i];
    }
    
    vector< vector<int> > ticket(j_jobs);
    for (unsigned int i = 0; i < j_jobs; i += 1)
    {
        int ni; cin >> ni;
        ticket[i].resize(ni);
        for (unsigned int t = 0; t < ticket[i].size(); t += 1)
        {
            cin >> ticket[i][t];
        }
    }
    
    //construct graph
    int V = j_jobs + z_zones + 2;
    int source = V-2;
    int target = V-1;
	Graph G(V);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
	
	//add edges from source to jobs, and between jobs and tickets
    for (unsigned int j = 0; j < j_jobs; j += 1)
    {
        eaG.addEdge(source, j, profit[j]);
        for (unsigned int t = 0; t < ticket[j].size(); t += 1)
        {
            int tn = ticket[j][t];
            eaG.addEdge(j, j_jobs + tn, INF_CAP);
        }
    }
    
    //add edges between tickets and target
    for (unsigned int z = 0; z < z_zones; z += 1)
    {
        eaG.addEdge(z + j_jobs, target, cost[z]);
    }
    
    //compute maxflow
    long flow = boost::push_relabel_max_flow(G, source, target);
    
	cout << total_profit - flow << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


