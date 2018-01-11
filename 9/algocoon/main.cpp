
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Namespaces
using namespace std;


// BGL Graph definitions
// ===================== 
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e; 
    }
};

void testcases() {
    int n, m;
    cin >> n >> m;
    
	Graph G(n);
	int source = 0;
	int target = n-1;
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
	
	for (unsigned int i = 0; i < m; i += 1)
	{
	    int from, to, cost;
	    cin >> from >> to >> cost;
	    
	    eaG.addEdge(from, to, 1, cost);
	}
    
    boost::successive_shortest_path_nonnegative_weights(G, source, target);
    
    // BFS to find vertex set S
	std::vector<int> vis(n, false); // visited flags
	std::queue<int> Q; // BFS queue (from std:: not boost::)
	vis[source] = true; // Mark the source as visited
	Q.push(source);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		OutEdgeIt ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, G);
			// Only follow edges with spare capacity
			if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
			Q.push(v);
		}
	}
    
    //go over all vertices of S and their outgoing edges, sum up cost of edges leaving S
    int total_cost = 0;
    for (unsigned int i = 0; i < n; i += 1)
    {
        if(vis[i]) {
            //check the outgoing edges of i
            OutEdgeIt ebeg, eend;
		    for (boost::tie(ebeg, eend) = boost::out_edges(i, G); ebeg != eend; ++ebeg) {
			    const int v = boost::target(*ebeg, G);
			    // Only follow edges with spare capacity
			    int c = weightmap[*ebeg];
                if(not vis[v] && c > 0) total_cost += c;
		    }
		    
		    //cout << i << " ";
        }
    }
    //cout << endl;
    cout << total_cost << endl;

}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


