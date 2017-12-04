#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
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
    }
};


void testcase() {
    const int MAX_PRICE = 20;
	int n_days;
	cin >> n_days;

    // build Graph
    const int V = n_days;
    const Vertex source = V+1;
    const Vertex target = V+2;
    Graph G(V);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    //read supply
    for (int i = 0; i < n_days; ++i)
    {   
        int amount, cost;
        cin >> amount >> cost;

        eaG.addEdge(source, i, amount, cost);
    }


    //read demand
    int total_students = 0;
    for (int i = 0; i < n_days; ++i)
    {
        int students, price;
        cin >> students >> price;
        total_students += students;

        eaG.addEdge(i, target, students, -1 * price + MAX_PRICE);
    }


    // read transitions between days
    for (int i = 0; i < n_days-1; ++i)
    {
        int amount, cost;
        cin >> amount >> cost;

        eaG.addEdge(i, i+1, amount, cost);
    }


    //compute flow
/*    int flow = boost::push_relabel_max_flow(G, source, target);
    boost::cycle_canceling(G);*/

    boost::successive_shortest_path_nonnegative_weights(G, source, target);
    int flow=0;

    //production cost
    int total_cost=0;
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source,G), G); 
        e != eend; ++e) {
        total_cost += (capacitymap[*e] - rescapacitymap[*e]) * weightmap[*e];
        flow += capacitymap[*e] - rescapacitymap[*e];
    }

/*    cout << "production cost: " << total_cost << endl;
*/
    //freezer cost
    int total_fcost=0;
    for (int i = 1; i < n_days; ++i)
    {
        for(boost::tie(e, eend) = boost::out_edges(boost::vertex(i,G), G); 
            e != eend; ++e) {
            if(boost::target(*e, G) == boost::vertex(i-1, G)) {
                total_fcost += (rescapacitymap[*e] - capacitymap[*e]) * -1 * weightmap[*e];
            }
        }
    }

/*    cout << "freezer cost: " << total_fcost << endl;
*/
    //compute total income for flow
    int total_income=0;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(target,G), G); 
        e != eend; ++e) {
        total_income += (rescapacitymap[*e] - capacitymap[*e]) * (weightmap[*e] + MAX_PRICE);
    }


/*    cout << "income: " << total_income << endl;
*/
    // output
    if(flow < total_students) {
        cout << "impossible ";
    }
    else {
        cout << "possible ";
    }

    cout << flow << " " << total_income - total_cost - total_fcost << endl;
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