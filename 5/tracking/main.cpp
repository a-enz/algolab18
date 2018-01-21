// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

// Namespaces
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
typedef boost::property_map<Graph, boost::edge_weight_t>::type  WeightMap;  // property map to access the interior property edge_weight_t

struct Road {
    int from, to, dist, has_river;
};

// Functions
// ========= 
void testcases() {
    int n_cities, m_roads, k, x_start, y_end;
    cin >> n_cities >> m_roads >> k >> x_start >> y_end;

    vector<Road> roads(m_roads);
    for (size_t i = 0; i < m_roads; ++i)
    {
        int from, to, dist, has_river;
        cin >> from >> to >> dist >> has_river;
        roads[i] = {from, to, dist, has_river};
    }

    int V = n_cities * (k+1);
    int source = x_start;
    int target = n_cities * k + y_end;
    Graph G(V); // creates an empty graph on n vertices
    WeightMap weightmap = boost::get(boost::edge_weight, G);

    for (int copy = 0; copy < k+1; ++copy)
    {
        for (int i = 0; i < m_roads; ++i)
        {
            Edge e; bool success;
            if(roads[i].has_river && copy < k) {
                //connect to upper copy layer of the graph
                boost::tie(e, success) = 
                    boost::add_edge(n_cities * copy + roads[i].from, 
                                    n_cities * (copy+1) + roads[i].to, G);
                weightmap[e] = roads[i].dist;
                //from both ends
                boost::tie(e, success) = 
                    boost::add_edge(n_cities * copy + roads[i].to, 
                                    n_cities * (copy+1) + roads[i].from, G);
                weightmap[e] = roads[i].dist;
            }
            else {
                //connect on the same layer                
                boost::tie(e, success) = 
                    boost::add_edge(n_cities * copy + roads[i].from, 
                                    n_cities * copy + roads[i].to, G);
                weightmap[e] = roads[i].dist;
                //from both ends
                boost::tie(e, success) = 
                    boost::add_edge(n_cities * copy + roads[i].to, 
                                    n_cities * copy + roads[i].from, G);
                weightmap[e] = roads[i].dist;
            }
        }
    }

    std::vector<int> distmap(V);        // We will use this vector as an Exterior Property Map: Vertex -> Distance to source
    boost::dijkstra_shortest_paths(G, source, // We MUST provide at least one of the two maps
        boost::distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));  // distance map as Named Parameter

    assert(distmap[target] < INT_MAX);
    cout << distmap[target] << endl;
}

// Main function looping over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;   // First input line: Number of testcases.
    while(T--)  testcases();
    return 0;
}

