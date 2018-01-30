#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

//undir graph for the matching
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,     // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
        boost::no_property,             // interior properties of vertices  
        boost::no_property      // interior properties of edges
        >                   UndirGraph;
typedef boost::graph_traits<UndirGraph>::vertex_descriptor       UVertex;     // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  

// Functions
// ========= 
bool too_small(int time, int d, int cap, const vector< vector<int> >& asmap) {
    //build bipartite graph with edges below given time
    //  'edge time' + 'enter delay d' == 'time'
    int a_agents = asmap.size();
    int s_shelters = asmap[0].size();
    int V = a_agents + cap * s_shelters;
    UndirGraph G(V);
    for (int a = 0; a < a_agents; ++a)
    {
        for (int s = 0; s < s_shelters; ++s)
        {   
            for (int c = 1; c <= cap; ++c)
            {
                if(asmap[a][s] <= time - c * d)
                    boost::add_edge(a, a_agents + s + s_shelters * (c-1), G);
            }
        }
    }

    //compute matching
    vector<UVertex> matemap(V);     // We MUST use this vector as an Exterior Property Map: Vertex -> Mate in the matching
    boost::edmonds_maximum_cardinality_matching(G, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, G)));
    int matchingsize = matching_size(G, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, G)));

    return matchingsize < a_agents;
}


void testcases() {
    int n_pos, m_slopes, a_agents, s_shelters, c_cap, d_time;
    cin >> n_pos >> m_slopes >> a_agents >> s_shelters >> c_cap >> d_time;
    
    int V = n_pos;	
    Graph G(V);	// creates an empty graph on n vertices
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	
	for (unsigned int i = 0; i < m_slopes; i += 1)
	{
	    char w; int x, y, z;
	    cin >> w >> x >> y >> z;
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
        int start;
        cin >> start; //start of agent i
	    boost::dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
            boost::distance_map(boost::make_iterator_property_map(agent_distmap[i].begin(), boost::get(boost::vertex_index, G))));  // distance
	}

    vector< vector<int> > asmap(a_agents, vector<int>(s_shelters));
    for (int i = 0; i < s_shelters; ++i)
    {
        int pos;
        cin >> pos;
        for (int a = 0; a < a_agents; ++a)
            asmap[a][i] = agent_distmap[a][pos];
    }

    //now do binary search, checking if every agent
    //can be matched with a shelter given the bomb timer
    int lmin = 1, lmax = 1;
    while(2*lmax <= INT_MAX && too_small(lmax, d_time, c_cap, asmap)) {
        lmin = lmax;
        lmax *= 2;
    }

    while(lmin < lmax) {
        int mid = lmin + (lmax - lmin) / 2;

        if(too_small(mid, d_time, c_cap, asmap))
            lmin = mid+1;
        else
            lmax =   mid;
    }

    cout << lmin << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


