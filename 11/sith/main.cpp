// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <set>

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
typedef Triangulation::Edge_circulator                          Edge_circulator;

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
    int n; long r; 
    cin >> n >> r;
    
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
        if(t.segment(eit).squared_length() <= r * r) {
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

    //build data structure to query all vertices of a component easily
    vector< set<int> > component_vertices(ncc, set<int>());
    for (int i = 0; i < n; ++i)
    {
        component_vertices[componentmap[i]].insert(i);
    }


    //check the sizes of the components, maybe there is already a solution
    int max_size = 0;
    for (int i = 0; i < ncc; ++i)
    {
        max_size = max(max_size, (int) component_vertices[i].size());
    } 


    if(max_size >= l) {
        cout << l << endl;
        return;
    }

    //add planets back in to check if connected components grow larger/equal
    //than the allowed number of days: l-1, l-2, ..., 0 (order of days and planets)

    for (int day = l-1; day > 0; --day) //can't take planet 0
    {
        //add to the triangulation and check the neighboring planets
        Vertex v = t.insert(pts[day]);
        v->info() = day;

        set<int> neighbor_comp;
        Edge_circulator c = t.incident_edges(v);
        do {
            if (not t.is_infinite(c) && t.segment(c).squared_length() <= r * r) {
                //extract the component of neighboring vertice (planet)
                Vertex v1 = c->first->vertex((c->second + 1) % 3);
                Vertex v2 = c->first->vertex((c->second + 2) % 3);

                int u = v1->info();
                int v = v2->info();

                assert(u == day || v == day);

                //newly added planet has lowest index of all so far added
                int nv = max(u, v);

                //store component of encountered neighbor
                neighbor_comp.insert(componentmap[nv]);
            }
        } while (++c != t.incident_edges(v)); 


        //merge encountered components into one component and check its size

        //The single vertex component of 'v' does not connect to any other components
        //nothing to merge
        if(neighbor_comp.empty()) {
            if(max_size >= day) {
                cout << day << endl;
                return;
            }
            continue;
        }


        //pick the dominant component for the merge
        int dominant_comp = *neighbor_comp.begin();
        set<int>* dominant_vertices = &component_vertices[dominant_comp];

        //add the new vertex 'v'
        componentmap[day] = dominant_comp;
        dominant_vertices->insert(day);

        //add the discovered neighboring vertices
        for(auto comp_it = neighbor_comp.begin();
            comp_it != neighbor_comp.end();
            comp_it++) {
            if(dominant_comp != *comp_it) {
                //update componentmap
                for(int nb_planet : component_vertices[*comp_it]) {
                    componentmap[nb_planet] = dominant_comp;
                }

                //update component_vertices
                dominant_vertices->insert(component_vertices[*comp_it].begin(),
                                        component_vertices[*comp_it].end());
                //clear the old set, not really necessary for correctness
                //but keeps data structure smaller
                component_vertices[*comp_it].clear();
            }
        }

        //update maximal size 
        max_size = max(max_size, (int) dominant_vertices->size());

        if(max_size >= day) {
            cout << day << endl;
            return;
        }
    }



    
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	{
        testcases();
    }
	return 0;
}


