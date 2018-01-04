#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,     // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
        boost::no_property,             // interior properties of vertices  
        boost::property<boost::edge_weight_t, int>      // interior properties of edges
        >                   Graph;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,     // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
        boost::no_property            // interior properties of vertices  
        >                   UndirGraph;
typedef boost::graph_traits<Graph>::edge_descriptor     Edge;       // Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor       Vertex;     // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  
typedef boost::graph_traits<Graph>::edge_iterator       EdgeIt;     // to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator       OutEdgeIt;  // to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type  WeightMap;  // property map to access the interior property edge_weight_t


bool too_small(int duration, const vector< vector<int> >& distances, int n_agents, int n_shelters) {
    //construct the graph
    UndirGraph T(n_agents + n_shelters);
    for (int i = 0; i < n_agents; ++i)
    {
        for (int j = 0; j < n_shelters; ++j)
        {
            int dist = distances[i][j];
            if(dist <= duration) {
                boost::add_edge(i, j+n_agents, T);
            }
        }
    }

    //compute matching
    std::vector<int> matemap(n_agents+n_shelters);
    boost::edmonds_maximum_cardinality_matching(T, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, T)));

    int matchingsize = boost::matching_size(T, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, T)));

    //return false if not all agents matched
    return matchingsize < n_agents; 
}

void testcase() {
	int n_pos, m_edges, n_agents, n_shelters, c_shelter, d;
    cin >> n_pos >> m_edges >> n_agents >> n_shelters >> c_shelter >> d;

    vector<int> pos_agents(n_agents);
    vector<int> pos_shelters(n_shelters);

    //construct graph of slopes and lifts
    Graph G(n_pos);
    WeightMap weightmap = boost::get(edge_weight, G);

    for (int i = 0; i < m_edges; ++i)
    {
        char w;
        int x, y, t;
        cin >> w >> x >> y >> t;
        Edge e; bool success;   
        tie(e, success) = boost::add_edge(x, y, G);
        weightmap[e] = t;

        if(w == 'L') { //add the reverse edge too
            tie(e, success) = boost::add_edge(y, x, G);
            weightmap[e] = t; 
        }
    }

    //read agent pos
    for (int i = 0; i < n_agents; ++i)
    {
        cin >> pos_agents[i];
    }

    //read shelter pos

    for (int i = 0; i < n_shelters; ++i)
    {
        cin >> pos_shelters[i];
    }

    //for each agent do dijkstra to find out which shelters 
    //are reachable in what time
    vector< vector<int> > agent_dist(n_agents, vector<int>(n_pos));
    for (int i = 0; i < n_agents; ++i)
    {
        boost::dijkstra_shortest_paths(G, pos_agents[i], // We MUST provide at least one of the two maps
        boost::distance_map(boost::make_iterator_property_map(agent_dist[i].begin(), boost::get(boost::vertex_index, G))));
    }

    vector< vector<int> > agent_shelter_dist(n_agents, vector<int>(n_shelters));
    int max_dist = 0;
    for (int i = 0; i < n_agents; ++i)
    {
        for (int j = 0; j < n_shelters; ++j)
        {
            agent_shelter_dist[i][j] = agent_dist[i][j]; //unreachable are set to INT_MAX
            int dist = agent_dist[i][j];
            if(dist != INT_MAX && dist > max_dist) {
                max_dist = dist;
            }
        }
    }

    
    //binary search on the distance (time) to reach shelter
    //if no pairing of agent -> shelter is possible then the time is too small
    
    int lmin = 0, lmax = max_dist;
    while(lmin != lmax) {
        int p = lmin + (lmax - lmin)/2;
        if(not too_small(p, agent_shelter_dist, n_agents, n_shelters)) //ms < n_agents
            lmin = p+1;
        else //ms == n_agents
            lmax = p;
    }

    cout << lmax << endl;



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