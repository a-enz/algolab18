#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >   Graph;
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type        EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type   ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type     ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor           Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor         Edge;
typedef boost::graph_traits<Graph>::edge_iterator           EdgeIt;

typedef boost::graph_traits<Graph>::out_edge_iterator           OutEdgeIt;


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    ReverseEdgeMap  &revedgemap;

public:
    // to initialize the Object
    EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
        G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

    // to use the Function (add an edge)
    void addEdge(int from, int to, long capacity) {
        Edge e, rev_e;
        bool success;
        boost::tie(e, success) = boost::add_edge(from, to, G);
        boost::tie(rev_e, success) = boost::add_edge(to, from, G);
        capacitymap[e] = capacity;
        capacitymap[rev_e] = 0; // reverse edge has no capacity!
        revedgemap[e] = rev_e;
        revedgemap[rev_e] = e;
    }
};


void testcase() {
    int n_gs, n_sat, n_links;
    cin >> n_gs >> n_sat >> n_links;

    // Create graph and maps
    int V = n_gs + n_sat + 2;
    int start_gs_vertices = 0;
    int start_sat_vertices = n_gs;
    Vertex source = n_gs + n_sat;
    Vertex target = n_gs + n_sat + 1;
    Graph G(V);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    // Add links from source to ground stations
    for (int i = start_gs_vertices; i < n_gs; ++i)
    {
        eaG.addEdge(source, i, 1);
    }

    // Add links from satellite to sink
    for (int i = start_sat_vertices; i < n_gs+n_sat; ++i)
    {
        eaG.addEdge(i, target, 1);
    }


    // Add links from ground stations to satellites
    for(int i=0; i<n_links; i++) 
    {
        int from, to; 
        cin >> from >> to;

        eaG.addEdge(from, to + start_sat_vertices, 1);
    }

    // Compute flow
    int flow = boost::push_relabel_max_flow(G, source, target);

    // Do bfs traversal and mark visited nodes
    std::vector<int> vis(V, false); // visited flags
    std::queue<int> Q; // BFS queue (from std:: not boost::)
    vis[source] = true; // Mark the source as visited
    Q.push(source);
    while (!Q.empty()) {
        const int u = Q.front();
        Q.pop();
        OutEdgeIt ebeg, eend;
        for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
            const int v = boost::target(*ebeg, G);
            // Only follow edges with spare capacity
            if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
            vis[v] = true;
            Q.push(v);
        }
    }

    //Read out solution from visited flags
    // we obviously ignore source and target

    //read vis flags for gs (LEFT)
    vector<int> marked_gs;
    for (int i = start_gs_vertices; i < n_gs; ++i)
    {
        if(not vis[i]) marked_gs.push_back(i);
    }

    //read vis flags for sat (RIGHT)
    vector<int> marked_sat;
    for (int i = start_sat_vertices; i < n_sat+n_gs; ++i)
    {
        if(vis[i]) marked_sat.push_back(i - start_sat_vertices);
    }

    // write output
    cout << marked_gs.size() << " " << marked_sat.size() << endl;

    for (int i = 0; i < marked_gs.size(); ++i)
    {
        cout << marked_gs[i] << " "; 
    }

    for (int i = 0; i < marked_sat.size(); ++i)
    {
        cout << marked_sat[i] << " ";
    }

    if(marked_gs.size() != 0 || marked_sat.size() != 0) 
        cout << endl;
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
