
// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
// Namespaces
using namespace std;


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

// Idea:
// MST
// for each edge in MST:
//  compute shortest path if edge removed and all other edges in MST have weight=0
//  compute dist of shortest path, remember added length to new ""MST""
// output minimum cost


// Functions
// ========= 
void testcases() {
	//read number of planets and index of start vertice
	int n, start;
	cin >> n >> start;
	
	//bild graph
	int V=n;
	Graph G(V);	// creates an empty graph on n vertices
	WeightMap weightmap = boost::get(boost::edge_weight, G);	
	
	//read connection costs: 0-indexing
	vector< vector<int> > cost(n, vector<int>(n));
	int max_c = 0;
	for (int i = 0; i < n-1; i += 1)
	{
	    for (int j = i+1; j < n; j += 1)
	    {
	        cin >> cost[i][j]; //j >= i+1, i <- {0,...,n-2}
	        Edge e; bool success;
	        boost::tie(e, success) = boost::add_edge(i, j, G);
	        weightmap[e] = cost[i][j];
	        if(max_c < cost[i][j]) {
	            max_c = costs[i][j];
	        }
	    }
	}
	
	
	// Prim minimum spanning tree
	// ==========================
	std::vector<Vertex> primpredmap(V);	// We MUST use this vector as an Exterior Property Map: Vertex -> Prim Predecessor
	boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)),	// Prim from user-defined start,
			boost::root_vertex(start-1)); //start is in 1-indexing!!
			
    //build the complete graph again with the following changes
    // - "remove" the edge (i, primpredmap[i]) by increasing the weight a lot
    // - set weight of MST edges to 0
    
	Graph G_path(V);	// creates an empty graph on n vertices
	WeightMap weightmap_path = boost::get(boost::edge_weight, G_path);	

	for (int i = 0; i < n-1; i += 1)
	{
	    for (int j = i+1; j < n; j += 1)
	    {
	        Edge e; bool success;
	        boost::tie(e, success) = boost::add_edge(i, j, G_path);
	        if(primpredmap[i] == j || primpredmap[j] == i) {
	            weightmap_path[e] = 0;
	        } else { 
	            weightmap_path[e] = cost[i][j];
	        }
	    }
	}
	
	
	//iterate over all MST edges and compute alternative shortest path
	for (int i = 0; i < V; ++i) {
		if (primpredmap[i] != i) {
			Edge e; bool success;
			boost::tie(e, success) = boost::edge(i, primpredmap[i], G_path);
			weightmap_path[e] = max_c + 1;
			
		}
	}
		
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}
