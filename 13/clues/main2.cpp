#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

// Namespaces
using namespace std;

struct Info {
    int color, comp;
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<Info, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Finite_vertices_iterator  Vertex_iterator;
typedef Triangulation::Finite_edges_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle VH;
typedef Triangulation::Vertex_circulator VC;
typedef K::Point_2  P;


bool no_interference(Triangulation& t, double rr) {
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
        if(t.segment(e).squared_length() <= rr) 
            return false;
    
    return true;
}


bool has_valid_network(Triangulation& t, double rr) {
    //assume all info fields have been initialized to (-1, -1) = (color, component)
    
    int cnumber = 0;
    for (Vertex_iterator vi = t.finite_vertices_begin(); 
        vi != t.finite_vertices_end(); 
        ++vi) 
    {
        if(vi->info().comp != -1)
            continue; //already belongs to component
        else {
            cnumber++;
            //attempt bfs coloring
            vi->info() = {0, cnumber};
            queue<VH> Q;
            Q.push(vi);
            while(not Q.empty()) {
                VH parent = Q.front();
                Q.pop();
                VC child = t.incident_vertices(parent);
                do {
                    if (not t.is_infinite(child)) { 
                        K::FT dist = CGAL::squared_distance(parent->point(), child->point());
                        if(dist <= rr) { //in range
                            if(child->info().color == parent->info().color)
                                return false; //found invalid coloring
                            else if (child->info().comp == -1) { //found new vertex
                                child->info() = {1 - parent->info().color, cnumber};
                                Q.push(child);
                            }
                            //else: valid coloring found, do nothing
                        }
                    }
                } while (++child != t.incident_vertices(parent));  
            }
        }    
    }
    
    
    Triangulation tr0, tr1;
    for (Vertex_iterator vi = t.finite_vertices_begin(); 
        vi != t.finite_vertices_end(); 
        ++vi) 
    {
        int color = vi->info().color;
        assert(color == 1 || color == 0);
        
        if(color == 0)
            tr0.insert(vi->point());
        else 
            tr1.insert(vi->point());
    }
    
    return no_interference(tr0, rr) && no_interference(tr1, rr);
}


// Functions
// ========= 
void testcases() {
    size_t n_stations, m_clues; double rr;
    cin >> n_stations >> m_clues >> rr;
    
    rr = rr * rr;
    vector< pair<P, Info> > stations(n_stations);
    for (unsigned int i = 0; i < n_stations; i += 1)
    {
        int x, y;
        cin >> x >> y;
        Info def = {-1, -1};
        stations[i] = make_pair(P(x, y), def);
    }
    
    vector<P> as(m_clues);
    vector<P> bs(m_clues);
    for (unsigned int i = 0; i < m_clues; i += 1)
    {
        int x, y, u, v;
        cin >> x >> y >> u >> v;
        as[i] = P(x, y);
        bs[i] = P(u, v);
    }
    
    Triangulation t;
    t.insert(stations.begin(), stations.end());
    
    //check if the network can be two colored by
    //first two coloring the spanning forest of the triangulation
    //and then checking tr0 and tr1 for interference 
    //(where for each color we have a seperate triangulation)
    
    bool valid_network = has_valid_network(t, rr);
    
    for (unsigned int i = 0; i < m_clues; i += 1)
    {
        if(valid_network) {
            // check reachability
            K::FT d_direct = CGAL::squared_distance(as[i], bs[i]);
            VH va = t.nearest_vertex(as[i]);
            VH vb = t.nearest_vertex(bs[i]);
            if(d_direct <= rr) {
                cout << "y";
            }
            else if(CGAL::squared_distance(va->point(), as[i]) <= rr &&
                    CGAL::squared_distance(vb->point(), bs[i]) <= rr &&
                    va->info().comp == vb->info().comp) 
            {
                cout << "y";  
            }
            else 
                cout << "n";
        }
        else 
            cout << "n";
    }
    
    cout << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


