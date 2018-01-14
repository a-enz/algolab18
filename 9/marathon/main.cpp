#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <climits>
#include "prettyprint.hpp"
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;

// BGL Graph definitions
// ===================== 
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator



//global variables
int n_intersect;
Edge source_edge;


// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e;
        
        if(u == n_intersect) {
            source_edge = e;
        }
    }
};


void testcase() {
	int m_streets, start, finish;
	cin >> n_intersect >> m_streets >> start >> finish;
	
	// Create Graph and Map
	int V = n_intersect + 1;
	int source = n_intersect;
	int target = finish;
    Graph G(V);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
   
    for (unsigned int i = 0; i < m_streets; i += 1)
    {
        int from, to, capacity, length;
        cin >> from >> to >> capacity >> length;
        eaG.addEdge(from, to, capacity, length);
        eaG.addEdge(to, from, capacity, length);
        
        assert(from < n_intersect);
    }
    
    eaG.addEdge(source, start, 1, 0);

    //compute the shortest distance from start to finish by allowing only 1 unit of flow
    //then 'cost' == 'dist'
    boost::successive_shortest_path_nonnegative_weights(G, source, target);
    
    int distance;
    int flow = capacitymap[source_edge] - rescapacitymap[source_edge];

    if(flow == 1) { //there is a path connecting source and target
        distance = boost::find_flow_cost(G);
    }
    else {
        cout << 0 << endl;
        return;
    }
    
    //now do binary search on the size of the flow
    //conditioned on that |f| * dist(source, target) = cost(f)
    
    
    int lmin = 1, lmax = 1;
    
    //exponential search
    while(true) {
        //change source outflow
        capacitymap[source_edge] = lmax;
        
        //compute flow and cost
        boost::successive_shortest_path_nonnegative_weights(G, source, target);
    
        int cost = boost::find_flow_cost(G);
        int f = capacitymap[source_edge] - rescapacitymap[source_edge];
        
        
        if(not (lmax == f && f * distance == cost))
            break;
        
        lmax *= 2;
    }

    //binary search
    while (lmin != lmax) {
        int p = lmin + (lmax - lmin)/2;
        
        //change source outflow
        capacitymap[source_edge] = p;
        
        //compute flow and cost
        boost::successive_shortest_path_nonnegative_weights(G, source, target);
    
        int cost = boost::find_flow_cost(G);
        int f = capacitymap[source_edge] - rescapacitymap[source_edge];

        
        if (p == f && f * distance == cost)
            lmin = p + 1;
        else
            lmax = p;
    }
    
    cout << lmin-1 << endl;
    
    
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
