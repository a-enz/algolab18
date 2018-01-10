// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

//// Namespaces
using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel     K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>     Vb;
typedef CGAL::Triangulation_face_base_2<K>                      Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>             Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                  Triangulation;
typedef Triangulation::Finite_edges_iterator                    Edge_iterator;

typedef Triangulation::Vertex_handle                            Vertex;

typedef K::Point_2 P;

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>  Graph;

//        
//// Functions
// ========= 
void testcases() {
    int n, r; 
    cin >> n >> r;
    long r_sq = r * r;
    
    vector<P> pts(n);
    for (int i = 0; i < n; i += 1)
    {   
        int x, y;
        cin >> x >> y;
        pts[i] = P(x, y);
    }
    
    int l = n / 2;
    
    Triangulation t;
    t.insert(pts.begin() + l, pts.end());

    //add point order information to triangulation
    for (int i = l; i < n; ++i)
    {
        Vertex v = t.nearest_vertex(pts[i]);
        assert(v->point() == pts[i]);
        v->info() = i;
    }
    
    //convert to boost graph (undirected)
    Graph G(n);
    for(Edge_iterator eit = t.finite_edges_begin();
        eit != t.finite_edges_end();
        eit++) {

        //check if our spaceship can travel the distance
        //and add the edge to the boost graph if true
        if(t.segment(eit).squared_length() <= r_sq) {
            Vertex v1 = eit->first->vertex((eit->second + 1) % 3);
            Vertex v2 = eit->first->vertex((eit->second + 2) % 3);

            int u = v1->info();
            int v = v2->info();

            assert(not t.is_infinite(v1) && not t.is_infinite(v2));
            assert(v >= 0 && v < n);
            assert(u >= 0 && u < n);

            add_edge(v1->info(), v2->info(), G);
        }
    }


    //on the boost graph compute connected components
    vector<int> componentmap(n);
    int ncc = boost::connected_components(G, 
                boost::make_iterator_property_map(componentmap.begin(), 
                boost::get(boost::vertex_index, G)));

    std::vector<int> componentsize(ncc);
    for (int i = 0; i < n; ++i)
        ++componentsize[componentmap[i]];

    bool found = false;
    for (int i = 0; i < ncc; ++i)
    {
        if(componentsize[i] >= l) {
            found = true;
            break;
        }
    }

    if(found)
        cout << l << endl;
    else
        cout << "???\n";
    
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


