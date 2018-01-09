// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <map>
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

typedef map<int, int>::iterator     MapIt;


struct Booking {
    int from, to, dep, ar, profit;
};

const size_t INF_CAP = 20 * 100;
const size_t MAX_TIME = 100000;
const size_t MAX_P = 100;

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


void testcases() {
    size_t n_bookings, s_stations;
    cin >> n_bookings >> s_stations;
    
    vector<size_t> cars(s_stations);
    int total_cars = 0;
    for (size_t i = 0; i < s_stations; i += 1)
    {
        cin >> cars[i];
        total_cars += cars[i];
    }
    
    vector< map<int, int> > time_indexmap(s_stations, map<int, int>());
    vector<Booking> bookings(n_bookings);
    
    size_t vertex_counter = 0;
    //for each station, insert start and endtime
    for (size_t i = 0; i < s_stations; i += 1)
    {
        time_indexmap[i][0] = vertex_counter;
        vertex_counter++;
        time_indexmap[i][MAX_TIME] = vertex_counter;
        vertex_counter++;
    }
    
    for (size_t i = 0; i < n_bookings; i += 1)
    {   
        //store bookings
        int from, to, dep, ar, profit;
        cin >> from >> to >> dep >> ar >> profit;
        from--;
        to--; //convert to 0-indexing
        bookings[i] = {from, to, dep, ar, profit};
        
        map<int, int>* from_indices = &time_indexmap[from];
        map<int, int>* to_indices = &time_indexmap[to];
        
        //insert the times into the indexmap:
        if(from_indices->find(dep) == from_indices->end()) {
            from_indices->insert(make_pair(dep, vertex_counter));
            vertex_counter++;
        }
        
        if(to_indices->find(ar) == to_indices->end()) {
            to_indices->insert(make_pair(ar, vertex_counter));
            vertex_counter++;
        }
    }
    
//    
    //create the graph
    size_t V = vertex_counter + 2;
    Vertex source = vertex_counter;
    Vertex target = vertex_counter + 1;
    
    Graph G(V);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    //add edges for cars staying in place
    for (size_t i = 0; i < s_stations; i += 1)
    {
        map<int, int> indices = time_indexmap[i];
        if(indices.size() > 1) {
            MapIt from = indices.begin();
            for (MapIt to = indices.begin();
                to != indices.end();
                to++)
            {   
                int to_idx = to->second;
                int from_idx = from->second;
                
                if(to == indices.begin()) { //edge from source to first vertice of station
                    eaG.addEdge(source, to_idx, cars[i], 0);
                }
                else {
                    int duration = to->first - from->first;
                    eaG.addEdge(from_idx, to_idx, INF_CAP, duration * MAX_P);
                                        
                    //if 'to' is last entry in the indices map, also add edge to target vertice
                    
                    MapIt next = to;
                    next++;
                    if(next == indices.end()) {
                        eaG.addEdge(to_idx, target, INF_CAP, 0);
                    }
                    
                    from = to;
                }
            }
        }
    }
    
    //add edges for the bookings
    for (size_t i = 0; i < n_bookings; i += 1)
    {
        Booking b = bookings[i];
        int from = time_indexmap[b.from][b.dep];
        int to = time_indexmap[b.to][b.ar];
        
        int duration = b.ar - b.dep;

        eaG.addEdge(from, to, 1, duration * MAX_P - b.profit);
    }
    
    
    //Compute profit with cycle canceling
    boost::successive_shortest_path_nonnegative_weights(G, source, target);
        
    int flow = 0;
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source, G), G); e != eend; ++e) {
        flow += capacitymap[*e] - rescapacitymap[*e];
    }
    long cost = boost::find_flow_cost(G);
    
    cout << (long) MAX_P * (long) MAX_TIME * (long) flow - cost << endl;
    
    

}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


