// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

// Namespaces
using namespace std;


typedef CGAL::Exact_predicates_inexact_constructions_kernel     K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>     Vb;
typedef CGAL::Triangulation_face_base_2<K>                      Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                   Triangulation;
typedef pair<K::Point_2, int>           IPoint;
typedef Triangulation::Edge_iterator    Edge_iterator;
typedef Triangulation::Vertex_handle    Vh;
typedef boost::disjoint_sets_with_storage<> DS;


struct Edge {
    Edge(int _u, int _v, K::FT _sq_d):
        u(_u), v(_v), sq_d(_sq_d) {};
    int u, v;
    K::FT sq_d;
};

// Functions
// ========= 
void testcases() {
    size_t n, m;
    double p;
    cin >> n >> m >> p;
    
    vector<IPoint> jammers(n);
    for (unsigned int i = 0; i < n ; i += 1)
    {   
        int x, y;
        cin >> x >> y;
        jammers[i] = make_pair(K::Point_2(x, y), i);
    }
    
    Triangulation t;
    t.insert(jammers.begin(), jammers.end());
    
    //go over all edges of the triangulation and add them to a vector with length information
    vector<Edge> edges;
    edges.reserve(3*n);
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e){
        int u = e->first->vertex((e->second + 1) % 3)->info();
        int v = e->first->vertex((e->second + 2) % 3)->info();
        
        edges.push_back(Edge(u, v, t.segment(e).squared_length()));
    }
    
    //sort the edges
    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) -> bool{
       return a.sq_d < b.sq_d; 
    });
    
    DS dSp(n);
    //add all edges that are covered with p energy to the set object
    for (vector<Edge>::iterator ei = edges.begin(); 
    ei != edges.end() && ei->sq_d <= p; ei += 1)
    {
        dSp.union_set(ei->u, ei->v);
    }
    
    //go over all missions and find out which start / end points
    //are close enough to a jammer plus there exists a path between start and end,
    //which means start and end have to be in the same set in 'dSp'
    DS dSa(n), dSb(n);
    vector<Edge>::iterator ai=edges.begin(), bi=edges.begin();
    K::FT a=0, b=0;
    for (unsigned int i = 0; i < m; i += 1)
    {
        int x, y, u, v;
        cin >> x >> y >> u >> v;
        K::Point_2 start(x, y), end(u, v);
        
        Vh v1 = t.nearest_vertex(start);
        Vh v2 = t.nearest_vertex(end);
        
        K::FT d1 = CGAL::squared_distance(start, v1->point());
        K::FT d2 = CGAL::squared_distance(end, v2->point());
        
        K::FT d = 4 * std::max(d1, d2);
        
        if(d <= p && dSp.find_set(v1->info()) == dSp.find_set(v2->info())) {
            cout << "y";
            if(d > b) 
                b = d;
            //should also be in the same set in dSb
            for (; bi != edges.end() && 
                dSb.find_set(v1->info()) != dSb.find_set(v2->info()); bi += 1)
            {
                dSb.union_set(bi->u, bi->v);
            }
        }
        else {
            cout << "n";
        }
        
        //all missions should be covered by set a
        if(d > a) 
            a = d;
        for (; ai != edges.end() && 
            dSa.find_set(v1->info()) != dSa.find_set(v2->info()); ai += 1)
        {
            dSa.union_set(ai->u, ai->v);
        }
    }
    
    //bi and ai were incremented one more than needed
    if(bi != edges.begin() && (bi-1)->sq_d > b) 
        b = (bi-1)->sq_d;    
    if(ai != edges.begin() && (ai-1)->sq_d > a) 
        a = (ai-1)->sq_d;
    
    cout << endl;
    cout << std::setiosflags(std::ios::fixed) << std::setprecision(0) 
	    << a << endl << b << endl;

}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


