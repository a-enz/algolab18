#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

// Namespaces
using namespace std;

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

// Functions
// ========= 
void testcases() {
    int n_pos, m_slopes, a_agents, s_shelters, c_cap, d_time;
    cin >> n_pos >> m_slopes >> a_agents >> s_shelters >> c_cap >> d_time;
    
    int V = n_pos;	
    Graph G(V);	// creates an empty graph on n vertices
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	
	for (unsigned int i = 0; i < m_slopes; i += 1)
	{
	    char w;
	    int x, y, z;
	    cin >> w >> x >> y >> z
	    Edge e;
		boost::tie(e, std::ignore) = boost::add_edge(x, y, G);
		weightmap[e] = z;
		if(w == 'L') {
		    boost::tie(e, std::ignore) = boost::add_edge(y, x, G);
		    weightmap[e] = z;
		}
	    else
	        assert(w == 'S');
	}
	
	vector< vector<int> > agent_distmap(a_agents, vector<int>(V));
	for (unsigned int i = 0; i < a_agents; i += 1)
	{
	    
	}
	
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


