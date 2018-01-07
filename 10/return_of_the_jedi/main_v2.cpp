
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
			

    //iterate over all pairs of vertices (i, j) s.t i < j
    //for each pair check the following difference: c_new - c_old and get the smallest
    //c_new = cost[i][j]
    //c_old = highest cost of edge on path connecting i and j in MST
    int min_added_cost = INT_MAX;
	for (size_t i = 0; i < n-1; i += 1)
    {
        for (size_t j = i+1; j < n; j += 1)
        {
            //check if the current edge already in the MST, skip
            if(primpredmap[i] == j || primpredmap[j] == i) continue;


            int c_new = cost[i][j];
            int c_old;

            size_t ii = i, jj = j;

            // compute path by combining information of paths to the root
            vector<bool> visited(V, false);
            int max_cj = 0;

            //go to the root
            while(primpredmap[ii] != ii) {
                visited[ii] = true;
                ii = primpredmap[ii];
            }

            //go to root or until meeting the path i took to root
            while(primpredmap[jj] != jj && not visited[jj]) {
                int c;
                if(jj > primpredmap[jj]){
                    c = cost[primpredmap[jj]][jj];
                }
                else {
                    c = cost[jj][primpredmap[jj]];
                }

                //update max
                if(c > max_cj) {
                    max_cj = c;
                }

                jj = primpredmap[jj];
            }

            //go to root or until meeting j path
            ii = i;
            int max_ci = 0;
            while(primpredmap[ii] != ii && ii != jj) {
                int c;
                if(ii > primpredmap[ii]){
                    c = cost[primpredmap[ii]][ii];
                }
                else {
                    c = cost[ii][primpredmap[ii]];
                }

                //update max
                if(c > max_ci) {
                    max_ci = c;
                }

                ii = primpredmap[ii];
            }

            //update c_old
            if(max_ci < max_cj) {
                c_old = max_cj;
            }
            else {
                c_old = max_ci;
            }

            //check MST condition
            assert(c_new - c_old >= 0); //otherwise something went wrong

            if(c_new - c_old < min_added_cost) {
                min_added_cost = c_new - c_old;
            }

            //go backwards on both paths simult
        }
    }
    //cout << "min_added_cost " << min_added_cost << endl;
    cout << totalweight + min_added_cost << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}
