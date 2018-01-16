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
#include <cassert>
// BGL includes
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
    int n_objects, m_links, s_starts, d_dest;
    cin >> n_objects >> m_links >> s_starts >> d_dest;


    // Create Graph and Maps
    int V = 2*n_objects + 2;
    Vertex source = V-1;
    Vertex target = V-2;
    Graph G(V);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);

    //connect out node to in node
    //in nodes are indexed from [0, n_objects-1], out nodes [n_objects, 2*n_objects-1]
    for (int i = 0; i < m_links; ++i)
    {
        int from, to;
        cin >> from >> to;
        eaG.addEdge(from+n_objects, to, 1); //v_out -> v_in
    }

    //node internval connections (v_in to v_out)
    for (int i = 0; i < n_objects; ++i)
    {
        eaG.addEdge(i, i + n_objects, 1); //in -> out
    }

    //connect source to in node
    for (int i = 0; i < s_starts; ++i)
    {
        int s;
        cin >> s;
        eaG.addEdge(source, s, 1); //source -> in
    }

    //connect dest out node to target
    for (int i = 0; i < d_dest; ++i)
    {
        int d;
        cin >> d;
        eaG.addEdge(d+n_objects, target, 1); //out -> target
    }


    long flow = boost::push_relabel_max_flow(G, source, target);

    cout << flow << endl;

 
}

// Main function looping over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;   // First input line: Number of testcases.
    while(T--)  testcases();
    return 0;
}


