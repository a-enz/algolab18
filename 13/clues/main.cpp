// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <stack>
#include "prettyprint.hpp"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

// Namespaces
using namespace std;

struct VInfo {
    int color, component;
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<VInfo, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef Triangulation::Finite_vertices_iterator VertexIt;
typedef Triangulation::Vertex_circulator VertexCi;
typedef Triangulation::Vertex_handle VertexH;
typedef K::Point_2 P;

bool do_interfere(const Triangulation& t, const long& r) {
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        if(t.segment(e).squared_length() <= r * r)
            return true;
    }
    return false;
}

bool two_color_return_success(const Triangulation& t, const long& r) {
    //assume the Triangulation already has default info set
    //go over all finite vertices
    int components = 0;
    for (VertexIt v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) {
        if(v->info().component != -1) 
            continue; //already visited this vertice

        //otherwhise do dfs on this particular unvisited component        
        //update info of v to initialize this dfs
        v->info() = {0, components};

        //dfs
        stack<VertexH> S;
        S.push(v);
        do {
            //pop the top of the stack
            VertexH parent = S.top();
            S.pop();
            int c_parent = parent->info().color;
            assert(c_parent != -1);

            //go over all children
            VertexCi child  = t.incident_vertices(parent);
            do {
                //ensure child is reachable within radius r
                if (not t.is_infinite(child) &&
                    CGAL::squared_distance(parent->point(), child->point()) <= r*r) {

                    //check if child already colored with wrong color
                    int c_child = child->info().color;
                    if(c_child == c_parent) 
                        return false;
                    else if (c_child == -1){ //not yet colored
                        //color and visit child
                        child->info() = {1 - c_parent, components};
                        S.push(child);
                    }
                    //else colored with correct color, don't need to do anything
                }
            } while (++child != t.incident_vertices(parent));
        }
        while(not S.empty());

        //next vertex we start dfs at belongs to different component
        components++;
    }


    Triangulation t0, t1; //triangulation for color 0 and color 1
    for (VertexIt v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) {
        if(v->info().color == 0)
            t0.insert(v->point());
        else
            t1.insert(v->point());
    }

    //check that these triangulations don't have an edge that is too short
    //which would inmply two stations with the same color are interfering
    return (not do_interfere(t0, r)) && (not do_interfere(t1, r));
}

// Functions
// ========= 
void testcases() {
    int n_stations, m_clues;
    long r;
    cin >> n_stations >> m_clues >> r;

    vector< pair<P, VInfo> > stations(n_stations);
    VInfo def = {-1, -1};
    for (int i = 0; i < n_stations; ++i)
    {
        int x, y;
        cin >> x >> y;
        stations[i] = make_pair(P(x, y), def);
    }

    vector< pair<P,P> > clues(m_clues);
    for (int i = 0; i < m_clues; ++i)
    {
        int xa, ya, xb, yb;
        cin >> xa >> ya >> xb >> yb;
        clues[i] = make_pair(P(xa, ya), P(xb, yb));
    }

    //Triangulate the stations
    Triangulation t;
    t.insert(stations.begin(), stations.end());

    bool success = two_color_return_success(t, r);
    for (int i = 0; i < m_clues; ++i)
    {
        if(success) {
            //check if 'a' and 'b' of a clue connect to the same component
            //or 'a' and 'b' are in range of each other
            P pa = clues[i].first;
            P pb = clues[i].second;
            if(CGAL::squared_distance(pa, pb) <= r*r) {
                cout << "y";
            }
            else {
                VertexH va = t.nearest_vertex(pa);
                VertexH vb = t.nearest_vertex(pb);
                //in range of stations and stations connected?
                if(CGAL::squared_distance(pa, va->point()) <= r*r &&
                    CGAL::squared_distance(pb, vb->point()) <= r*r &&
                    va->info().component == vb->info().component) {
                    cout << "y";
                }
                else {
                    cout << "n";
                }
            }

        }
        else {
            cout << "n"; 
        }
    }
    cout << endl;
}

// Main function looping over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;   // First input line: Number of testcases.
    while(T--)  testcases();
    return 0;
}

