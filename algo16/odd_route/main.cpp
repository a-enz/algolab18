// ALGOLAB BGL Tutorial 3
// Code snippets demonstrating 
// - MinCostMaxFlow with negative edge costs using cycle_canceling
// - MinCostMaxFlow with negative edge costs using successive_shortest_path_nonnegative_weights

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl_mincostmaxflow.cpp -o bgl_mincostmaxflow; ./bgl_mincostmaxflow

// Includes
// ========
// STL includes
#include <iostream>
#include <cstdlib>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
// Namespaces
using namespace boost;
using namespace std;

// BGL Graph definitions
// ===================== 
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
    property<edge_capacity_t, long,
        property<edge_residual_capacity_t, long,
            property<edge_reverse_t, Traits::edge_descriptor,
                property <edge_weight_t, long> > > > > Graph;
// Interior Property Maps
typedef property_map<Graph, edge_capacity_t>::type      EdgeCapacityMap;
typedef property_map<Graph, edge_weight_t >::type       EdgeWeightMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type       ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor          Vertex;
typedef graph_traits<Graph>::edge_descriptor            Edge;
typedef graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, reverseE;
        tie(e, tuples::ignore) = add_edge(u, v, G);
        tie(reverseE, tuples::ignore) = add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w;
        capacitymap[reverseE] = 0;
        weightmap[reverseE] = -w;
        revedgemap[e] = reverseE; 
        revedgemap[reverseE] = e; 
    }
};

int n;

//row, column, 0/1 depending on in or out vertex per field
int get_index(int i, int isLengthOdd, int isValueOdd) {
    if(isLengthOdd == 0) {
        if(isValueOdd) {
            return i;
        } else {
            return i+n;
        }
    } else {
        if(isValueOdd) {
            return i+ 2*n;
        } else {
            return i+ 3*n;
        }
    }
}

void test() {
    int m;
    cin >> n >> m;

    // Create Graph and Maps
    Graph G(4*n);
    EdgeCapacityMap capacitymap = get(edge_capacity, G);
    EdgeWeightMap weightmap = get(edge_weight, G);
    ReverseEdgeMap revedgemap = get(edge_reverse, G);
    ResidualCapacityMap rescapacitymap = get(edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    Vertex src = add_vertex(G);
    Vertex sink = add_vertex(G);
    
    // Add edges to src and sink
    int start, end;
    cin >> start >> end;
    eaG.addEdge(src,get_index(start,0,0),1,0);
    eaG.addEdge(get_index(end,1,1),sink,1,0);
       
    //connect vertices
    for (unsigned int i = 0; i < m; i += 1) {
        int weight;
        cin >> start >> end >> weight;
        if(weight%2 == 0) {
            eaG.addEdge(get_index(start,0,0),get_index(end,1,0),1,weight);
            eaG.addEdge(get_index(start,0,1),get_index(end,1,1),1,weight);
            eaG.addEdge(get_index(start,1,0),get_index(end,0,0),1,weight);
            eaG.addEdge(get_index(start,1,1),get_index(end,0,1),1,weight);
        } else {
            eaG.addEdge(get_index(start,0,0),get_index(end,1,1),1,weight);
            eaG.addEdge(get_index(start,0,1),get_index(end,1,0),1,weight);
            eaG.addEdge(get_index(start,1,0),get_index(end,0,1),1,weight);
            eaG.addEdge(get_index(start,1,1),get_index(end,0,0),1,weight);
        }
    }

    // Run the algorithm
    successive_shortest_path_nonnegative_weights(G, src, sink);
    OutEdgeIt e, eend;
    int flow = 0;
    for(tie(e, eend) = out_edges(vertex(src,G), G); e != eend; ++e) {
        flow += capacitymap[*e] - rescapacitymap[*e];
    }
    if(flow == 1) {
        int cost = find_flow_cost(G);
        cout << cost << endl;
    } else {
        cout << "no" << endl;
    }
    
}

int main() {
    int T;
    cin >> T;
    while(T--)
        test();
    return 0;
}
