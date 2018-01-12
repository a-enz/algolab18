// ALGOLAB BGL Tutorial 1
// Tutorial example problem

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl-tutorial_problem.cpp -o bgl-tutorial_problem; ./bgl-tutorial_problem < bgl-tutorial_problem.in
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl-tutorial_problem.cpp -o bgl-tutorial_problem; ./bgl-tutorial_problem < bgl-tutorial_problem.in

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include "prettyprint.hpp"
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

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

int vertex_index_in(int i, int j) {
    assert(j <= i);
    return 2 * ((i+1) * i / 2 + j); 
}

int vertex_index_out(int i, int j) {
    return vertex_index_in(i, j) + 1;
}


//pair<int, int> ball_indices(int v) {
//    v = v / 2;
//    int i = 0;
//    
//    while((i+1)*i / 2 < v) {
//        i++
//    }
//    
//    i--;
//    return make_pair(i, v-i);
//}

bool valid_ball(int i, int j, int k_layers) {
    if(i < 0 || i >= k_layers)
        return false;
    else if(j < 0 || j > i)
        return false;
    else 
        return true;
}


// ========= 
void testcases() {
    int k_layers;
    cin >> k_layers;
    
    vector< vector<int> > B(k_layers, vector<int>(k_layers, -1));
    for (unsigned int i = 0; i < k_layers; i += 1)
    {
        for (unsigned int j = 0; j <= i; j += 1)
        {
            cin >> B[i][j];
        }
    }
    
    //Build graph
    int V = (k_layers + 1) * k_layers; // /2 * 2
    Graph G(V);	// creates an empty graph on n vertices
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	
    //go over all balls and connect them to neighbors
    for (int i = 0; i < k_layers; i += 1)
    {
        for (int j = 0; j <= i; j += 1)
        {   
            //cout << "connecting " << "(i=" << i << ", j=" << j << "):\n";
		    int out_vertex = vertex_index_out(i,j);
		    int in_vertex = vertex_index_in(i,j);
            //insert connections to neighbors
            for (int u = i-1; u <= i+1; u += 1)
            {
                for (int v = j-1; v <= j+1; v += 1)
                {
                    if((u != i || v != j) && 
                        not (u == i-1 && v == j+1) &&
                        not (u == i+1 && v == j-1) &&
                        valid_ball(u, v, k_layers)) {
                        
		                Edge e;
		                int target = vertex_index_in(u, v);
                        boost::tie(e, std::ignore) = boost::add_edge(out_vertex, target, G);
                        weightmap[e] = 0;
                        //cout << "    to: " << u << ", " << v << endl;
                    }
                }
            }
            
            //insert connection for the 'node'
            Edge e;
            boost::tie(e, std::ignore) = boost::add_edge(in_vertex, out_vertex, G);
            weightmap[e] = B[i][j];
        }
    }
    
    
    //do shortest paths 3 times, starting from each corner
    vector< vector<int> > distmap(3, vector<int>(V));
    boost::dijkstra_shortest_paths(G, vertex_index_out(0, 0), 
		boost::distance_map(boost::make_iterator_property_map(distmap[0].begin(), boost::get(boost::vertex_index, G))));	
		
    boost::dijkstra_shortest_paths(G, vertex_index_out(k_layers-1, 0), 
		boost::distance_map(boost::make_iterator_property_map(distmap[1].begin(), boost::get(boost::vertex_index, G))));
		
    boost::dijkstra_shortest_paths(G, vertex_index_out(k_layers-1, k_layers-1), 
		boost::distance_map(boost::make_iterator_property_map(distmap[2].begin(), boost::get(boost::vertex_index, G))));
		
		
	//now iterate over vertices and compute sum of minimum distances
	int min_burden = INT_MAX;
	for (unsigned int i = 1; i < V; i += 2)
	{
	    min_burden = min(min_burden, distmap[0][i] + distmap[1][i] + distmap[2][i]);
	}
	
	cout << min_burden << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


