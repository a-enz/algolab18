#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <cmath>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
// Namespaces
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

// Functions
// ========= 
void testcases() {
    int l_loc, p_paths;
    cin >> l_loc >> p_paths;

    int V = l_loc+2;
    int source = V-2;
    int target = V-1;
    Graph G(V);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    vector<int> demand(l_loc);
    vector<int> supply(l_loc);
    for (int i = 0; i < l_loc; ++i)
    {
        cin >> supply[i] >> demand[i];
    }

    for (int i = 0; i < p_paths; ++i)
    {
        int from, to, c_min, c_max;
        cin >> from >> to >> c_min >> c_max;

        supply[to] += c_min;
        demand[from] += c_min;

        eaG.addEdge(from, to, c_max - c_min);
    }

    for (int i = 0; i < l_loc; ++i)
    {
        eaG.addEdge(source, i, supply[i]);
        eaG.addEdge(i, target, demand[i]);
    }

    long flow = boost::push_relabel_max_flow(G, source, target);

    long total_demand = 0;
    for (int i = 0; i < l_loc; ++i)
    {
        total_demand += demand[i];
    }

    if(total_demand > flow)
        cout << "no\n";
    else
        cout << "yes\n";
}

// Main function looping over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;   // First input line: Number of testcases.
    while(T--)  testcases();
    return 0;
}

