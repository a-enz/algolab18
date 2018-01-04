#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,     // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
        boost::no_property,             // interior properties of vertices  
        boost::property<boost::edge_weight_t, int>      // interior properties of edges
        >                   Graph;
typedef boost::graph_traits<Graph>::edge_descriptor     Edge;       // Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor       Vertex;     // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  
typedef boost::graph_traits<Graph>::edge_iterator       EdgeIt;     // to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator       OutEdgeIt;  // to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type  WeightMap;  // property map to access the interior property edge_weight_t



void testcase() {
	int n_pos, m_edges, n_agents, n_shelters, c_shelter, d;
    cin >> n_pos >> m_edges >> n_agents >> n_shelters >> c_shelter >> d;

    vector<int> pos_agents(n_agents);
    vector<int> pos_shelters(n_shelters);

    Graph G(n_pos);
    WeightMap weightmap = get(edge_weight, G);

    for (int i = 0; i < m_edges; ++i)
    {
        int w, x, y, t;
        Edge e; bool success;   
        tie(e, success) = add_edge(v, u, G);
        weightmap[e] = w;

        if(w == 'L') { //add the reverse edge too
            tie(e, success) = add_edge(u, v, G);
            weightmap[e] = w; 
        }
    }


	//write output values for testcase
		cout << sum << '\n';
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