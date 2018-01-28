#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <map>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;

typedef vector< map<int, int> >   IdxMap;


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

//global variables
const int MAX_TIME = 100000;
const int MAX_P = 100;
const int MAX_CARS = 1000;

struct Booking {
    int from, to, dep, arr, profit;
};

// Functions
// ========= 
void testcases() {
    int n_bookings, s_stations;
    cin >> n_bookings >> s_stations;

    vector<int> cars(s_stations);   
    IdxMap idx_mapping(s_stations); //map (station, time) -> vertex index
    int vertex_counter = 0;
    for (int i = 0; i < s_stations; ++i)
    {
        cin >> cars[i];
        //also initialize start and end times
        idx_mapping[i][0] = vertex_counter++;
        idx_mapping[i][MAX_TIME] = vertex_counter++;
    }

    vector<Booking> bookings(n_bookings);
    for (int i = 0; i < n_bookings; ++i)
    {   
        int f, t, d, a, p;
        cin >> f >> t >> d >> a >> p;
        f--;
        t--; //convert to 0 indexing
        bookings[i] = {f, t, d, a, p};

        //update index map for the newly received times
        if(idx_mapping[f].find(d) == idx_mapping[f].end())
            idx_mapping[f][d] = vertex_counter++;

        if(idx_mapping[t].find(a) == idx_mapping[t].end())
            idx_mapping[t][a] = vertex_counter++;
    }

    // Create Graph and Maps
    int V = vertex_counter + 2;
    int source = V-2;
    int target = V-1;
    Graph G(V);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    //from source provide inflow to each station as the amount of cars there at the start
    //and outflow to target
    for (int i = 0; i < s_stations; ++i)
    {
        eaG.addEdge(source, idx_mapping[i][0], cars[i], 0);
        eaG.addEdge(idx_mapping[i][MAX_TIME], target, MAX_CARS, 0);
    }


    //add all the edges for the bookings
    for (int i = 0; i < n_bookings; ++i)
    {
        int time_diff = bookings[i].arr - bookings[i].dep;
        assert(time_diff >= 1);
        eaG.addEdge(idx_mapping[bookings[i].from][bookings[i].dep], //from
                    idx_mapping[bookings[i].to][bookings[i].arr], //to
                    1,                                                  //cap
                    MAX_P * time_diff - bookings[i].profit);            //cost
    }

    //add all the edges for idle cars
    for (int i = 0; i < s_stations; ++i)
    {
        auto prevIt = idx_mapping[i].begin();
        auto nextIt = idx_mapping[i].begin();
        nextIt++;
        for (;
            nextIt != idx_mapping[i].end();
            nextIt++)
        {
            int time_diff = nextIt->first - prevIt->first;
            assert(time_diff > 0);
            eaG.addEdge(prevIt->second,
                        nextIt->second,
                        MAX_CARS,
                        MAX_P * time_diff);

            prevIt = nextIt;
        }
    }

    boost::successive_shortest_path_nonnegative_weights(G, source, target);
    long cost = boost::find_flow_cost(G);
    long flow = 0;
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source,G), G); e != eend; ++e) {
        flow += capacitymap[*e] - rescapacitymap[*e];
    }

    cout << flow * (long) MAX_TIME * (long) MAX_P - cost << endl;
}

// Main function looping over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;   // First input line: Number of testcases.
    while(T--)  testcases();
    return 0;
}

