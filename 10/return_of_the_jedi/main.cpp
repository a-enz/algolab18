
// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <assert.h>
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

void dfs(int v, int max_c, 
        vector<bool>& visited,
        const vector< vector<int> >& MST,
        const vector< vector<int> >& cost,
        vector<int>& max) {

    for(int u : MST[v]) {
        if(not visited[u]) {
            visited[u] = true;
            int new_max = max_c;
            if(cost[v][u] > new_max) new_max = cost[u][v];
            max[u] = new_max;
            dfs(u, new_max, visited, MST, cost, max);
        }
    }

}

// Functions
// ========= 
void testcases() {
	//read number of planets and index of start vertice
	size_t n, start;
	cin >> n >> start;
	
	//bild graph
	size_t V=n;
	Graph G(V);	// creates an empty graph on n vertices
	WeightMap weightmap = boost::get(boost::edge_weight, G);	
	
	//read connection costs: 0-indexing
	vector< vector<int> > cost(n, vector<int>(n));
	for (size_t i = 0; i < n-1; i += 1)
	{
	    for (size_t j = i+1; j < n; j += 1)
	    {
	        cin >> cost[i][j]; //j >= i+1, i <- {0,...,n-2}
            cost[j][i] = cost[i][j]; //just store the symetric part of matrix, easier to read out values
	        Edge e; bool success;
	        boost::tie(e, success) = boost::add_edge(i, j, G);
	        weightmap[e] = cost[i][j];
	    }
	}
	
	
	// Prim minimum spanning tree
	// ==========================
	std::vector<Vertex> primpredmap(V);	// We MUST use this vector as an Exterior Property Map: Vertex -> Prim Predecessor
	boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)),	// Prim from user-defined start,
			boost::root_vertex(start-1)); //start is in 1-indexing!!

    //mst cost
    int totalweight = 0;
    for (size_t i = 0; i < V; ++i) {
        if (primpredmap[i] != i) {
            Edge e; bool success;
            boost::tie(e, success) = boost::edge(i, primpredmap[i], G);
            totalweight += weightmap[e];
        }
    }

    //for each vertice compute a list of max weight edges on the path to every
    //other vertice. This can be done in O(n)

    //store mst in an adj matrix
    vector< vector<int> > MST(V, vector<int>());

    for (size_t i = 0; i < V; ++i)
    {
        if(primpredmap[i] != i) {
            MST[i].push_back(primpredmap[i]);
            MST[primpredmap[i]].push_back(i);
        }
    }

    //do dfs on MST for every node
    //and store list of max edge weights on path to every other node in MST
    vector< vector<int> > max_path_weight(V, vector<int>(V));
    for (size_t i = 0; i < V; ++i)
    {
        //dfs
        vector<bool> visited(V, false);
        dfs(i, 0, visited, MST, cost, max_path_weight[i]);
    }

    //go over all pairs of vertices and check for the minimum difference
    //between a direct edge and the maximal edge on the mst path
    int min_added_cost = INT_MAX;
    for (size_t i = 0; i < n-1; i += 1)
    {
        for (size_t j = i+1; j < n; j += 1)
        {
            //skip edges in the MST
            if(primpredmap[i] == j || primpredmap[j] == i) continue;

            int c_direct = cost[i][j];
            int c_path = max_path_weight[i][j];
            //cout << "path weight: " << max_path_weight[i][j] << endl;

            int diff = c_direct - c_path;
            assert(diff >= 0);

            if(diff < min_added_cost) {
                min_added_cost = diff;
            }
        }
    }

    cout << totalweight + min_added_cost << endl;
			
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}
