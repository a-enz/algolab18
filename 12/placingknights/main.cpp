// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include "prettyprint.hpp"
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
// Namespaces
using namespace std;

// BGL Graph definitions
// =====================
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::no_property		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;	

//Global variables
int n;

class EdgeAdder {
	Graph &G;
	vector< vector<int> > &indexmap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, vector< vector<int> > &indexmap):
		G(G), indexmap(indexmap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to) {
		boost::add_edge(from, to, G);
	}

	void addEdgeIfValid(int from_i, int from_j, int to_i, int to_j) {
		assert(indexmap[from_i][from_j] != -1);

		if(to_i < 0 || to_i >= n || to_j < 0 || to_j >= n)
			return;

		if(indexmap[to_i][to_j] == -1)
			return;

		int from = indexmap[from_i][from_j];
		int to = indexmap[to_i][to_j];

		addEdge(from, to);
	}
};


// Functions
// ========= 

void testcases() {
	cin >> n;

	vector< vector<int> > vertex_idx(n, vector<int>(n, -1));
	int v_counter = 0;
	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = 0; j < n; ++j)
		{	
			int board_intact;
			cin >> board_intact;
			if(board_intact) {
				vertex_idx[i][j] = v_counter;
				v_counter++;
			}
		}
	}

	//create graph
	int V = v_counter;
	Graph G(V);
	EdgeAdder eaG(G, vertex_idx);

	//connect interfeering positions
	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = 0; j < n; ++j)
		{
			if(vertex_idx[i][j] != -1) {
				eaG.addEdgeIfValid(i, j, i-1, j-2);
				eaG.addEdgeIfValid(i, j, i-1, j+2);

				eaG.addEdgeIfValid(i, j, i+1, j-2);
				eaG.addEdgeIfValid(i, j, i+1, j+2);

				eaG.addEdgeIfValid(i, j, i+2, j-1);
				eaG.addEdgeIfValid(i, j, i+2, j+1);

				eaG.addEdgeIfValid(i, j, i-2, j-1);
				eaG.addEdgeIfValid(i, j, i-2, j+1);
			}
		}
	}


	std::vector<Vertex> matemap(V);		// We MUST use this vector as an Exterior Property Map: Vertex -> Mate in the matching
	boost::edmonds_maximum_cardinality_matching(G, 
		boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, G)));

	int matchingsize = matching_size(G, 
		boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, G)));

	//cout << "matchingsize " << matchingsize << endl;
	//Königs theorem: size of maximum independent set == V - matchingsize
	cout << V - matchingsize << endl;

}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}

