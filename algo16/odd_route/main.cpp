#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include "prettyprint.hpp"
// Namespaces
using namespace std;


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,     // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
        boost::no_property,             // interior properties of vertices  
        boost::property<boost::edge_weight_t, int>      // interior properties of edges
        >                   Graph;
typedef boost::property_map<Graph, boost::edge_weight_t>::type  WeightMap;  // property map to access the interior property edge_weight_t
typedef boost::graph_traits<Graph>::edge_descriptor     Edge;

// Functions
// ========= 
void testcases() {
    int n_edges, m_edges, source, target;
    cin >> n_edges >> m_edges >> source >> target;

    //two 2-layer graphs
    int V = 2*n_edges;
    target = n_edges + target;
    Graph G(V), G_R(V);    // Creates an empty graph on V vertices
    WeightMap weightmap = get(boost::edge_weight, G);
    WeightMap weightmap_rev = get(boost::edge_weight, G_R);

    vector<int> looplen(V, 0);
    for (int i = 0; i < m_edges; ++i)
    {
        int from, to, w;
        cin >> from >> to >> w;
        Edge e;
        //create a graph to get distances from source
        tie(e, std::ignore) = add_edge(from, n_edges + to, G);
        weightmap[e] = w;
        tie(e, std::ignore) = add_edge(n_edges + from, to, G);
        weightmap[e] = w;

        //create a graph to get distances from target
        tie(e, std::ignore) = add_edge(to, n_edges + from, G_R);
        weightmap_rev[e] = w;
        tie(e, std::ignore) = add_edge(n_edges + to, from, G_R);
        weightmap_rev[e] = w;

        if(from == to)
            looplen[from] = w;
    }

    vector<int> distmap(V);     // We must use at least one of these
    vector<int> distmap_rev(V);

    boost::dijkstra_shortest_paths(G, source,
        boost::distance_map(boost::make_iterator_property_map(    // concatenated by .
                    distmap.begin(), get(boost::vertex_index, G))));

    boost::dijkstra_shortest_paths(G_R, target,
        boost::distance_map(boost::make_iterator_property_map(    // concatenated by .
                    distmap_rev.begin(), get(boost::vertex_index, G_R))));

    int min_odd_dist = INT_MAX;
    int node = -1;
    vector<int> distances;
    for (int i = 0; i < n_edges; ++i)
    {   
        int dist_src, dist_trg;
        //case 1) even/odd number of hops
        dist_src = distmap[i];
        dist_trg = distmap_rev[i];
        if(dist_src != INT_MAX && dist_trg != INT_MAX && 
            (dist_src + dist_trg) % 2 != 0) {
            min_odd_dist = min(min_odd_dist, dist_src + dist_trg);
        }    
        //case 2) odd/even number of hops
        dist_src = distmap[i+n_edges];
        dist_trg = distmap_rev[i+n_edges];
        if(dist_src != INT_MAX && dist_trg != INT_MAX && 
            (dist_src + dist_trg) % 2 != 0) {
            min_odd_dist = min(min_odd_dist, dist_src + dist_trg);
        }
        //case 3) odd/odd number of hops
        dist_src = distmap[i+n_edges];
        dist_trg = distmap_rev[i];
        if(dist_src != INT_MAX && dist_trg != INT_MAX) { 
            int dist = dist_src + dist_src + looplen[i];
            if(dist % 2 != 0)
                min_odd_dist = min(min_odd_dist, dist);
        }
        //case 3) even/even number of hops
        dist_src = distmap[i];
        dist_trg = distmap_rev[i+n_edges];
        if(dist_src != INT_MAX && dist_trg != INT_MAX) { 
            int dist = dist_src + dist_src + looplen[i];
            if(dist % 2 != 0)
                min_odd_dist = min(min_odd_dist, dist);
        }
    }

    if(min_odd_dist == INT_MAX)
        cout << "no\n";
    else
        cout << min_odd_dist << endl;
}

// Main function looping over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;   // First input line: Number of testcases.
    while(T--)  testcases();
    return 0;
}

