#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <cmath>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <boost/pending/disjoint_sets.hpp>

// Namespaces
using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>    Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle    Vertex_handle;
typedef boost::disjoint_sets_with_storage<>   UF;
typedef K::Point_2  P;

struct Edge {
    int u, v;
    K::FT sq_len;
};

// Functions
// ========= 
double ceil_to_double(const K::FT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void testcases() {
    int n_jammers, m_missions;
    double p_init;
    cin >> n_jammers >> m_missions >> p_init;
    
    vector< pair<P, int> > jammer(n_jammers);
    for (unsigned int i = 0; i < n_jammers; i += 1)
    {
        int x, y;
        cin >> x >> y;
        jammer[i] = make_pair(P(x, y), i);
    }
    
    vector<P> start(m_missions), end(m_missions);
    for (unsigned int i = 0; i < m_missions; i += 1)
    {
        int x, y, u, v;
        cin >> x >> y >> u >> v;
        start[i] = P(x, y);
        end[i] = P(u, v);
    }
    
    // Put all triangulation edges into a sorted by length vector
    Triangulation t;
    t.insert(jammer.begin(), jammer.end());
    
    vector<Edge> tr_edges;
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
        Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
        
        Edge edge = {v1->info(), v2->info(), CGAL::squared_distance(v1->point(), v2->point())};
        tr_edges.push_back(edge);
    }
    
    sort(tr_edges.begin(), tr_edges.end(), 
        [](const Edge& a, const Edge& b)->bool{
            return a.sq_len < b.sq_len;
        });
    
    
    //initialize union find
    UF ufa(n_jammers), ufb(n_jammers);
    for (int i = 0; i < n_jammers; i += 1)
    {
        ufa.make_set(i);
        ufb.make_set(i);
    }
    
    
    //insert edges and merge components of enpoints until edge length is larger than provided
    //energy allows
    auto eit_a = tr_edges.begin();
    for (; //allow eges of up to size (2*d)^2, where d = sqrt(w/4)
        eit_a != tr_edges.end() && eit_a->sq_len <= p_init; 
        eit_a += 1)
    {
        ufa.union_set(eit_a->u, eit_a->v);
    }
    
    
    //answer the y/n query
    vector<bool> is_possible(m_missions, false);
    for (unsigned int i = 0; i < m_missions; i += 1)
    {
        //check if start and end are in range of a jammer
        Vertex_handle vs = t.nearest_vertex(start[i]);
        Vertex_handle ve = t.nearest_vertex(end[i]);
        
        K::FT dist_start = CGAL::squared_distance(start[i], vs->point());
        K::FT dist_end = CGAL::squared_distance(end[i], ve->point());
        if(4 * dist_start > p_init || 4 * dist_end > p_init) {
            cout << "n";
            continue;
        }
        
        if(ufa.find_set(vs->info()) != ufa.find_set(ve->info())) {
            cout << "n";
            continue;
        }
        
        //else start and end are in range of jammers and
        //those jammers are in the same component
        is_possible[i] = true;
        cout << "y";   
    }
    
    cout << endl;
    
    //go through all the missions again, adding edges until 
    //components are connected as in 'ufa', taking note of the maximal distance necessary
    
    auto eit = tr_edges.begin();
    K::FT dist_a = 0, dist_b = 0;
    for (unsigned int i = 0; i < m_missions; i += 1)
    {
        Vertex_handle vs = t.nearest_vertex(start[i]);
        Vertex_handle ve = t.nearest_vertex(end[i]);
        K::FT dist_start = CGAL::squared_distance(start[i], vs->point());
        K::FT dist_end = CGAL::squared_distance(end[i], ve->point());
        
        if(is_possible[i]) {
            dist_a = max({dist_a, 4*dist_start, 4*dist_end});
            while(eit != tr_edges.end() && 
                ufb.find_set(vs->info()) != ufb.find_set(ve->info())) {
                
                ufb.union_set(eit->u, eit->v);
                dist_a = max(dist_a, eit->sq_len);
                eit++;
            }
        }
        else { //not possible, expand 'ufa'
            while(eit_a != tr_edges.end() && 
                ufa.find_set(vs->info()) != ufa.find_set(ve->info())) {
                
                ufa.union_set(eit_a->u, eit_a->v);
                dist_b = max(dist_b, eit_a->sq_len);
                eit_a++;
            }
        }
        
        
        dist_b = max({dist_a, dist_b, 4*dist_start, 4*dist_end});
    }
    
    
    cout << setiosflags(std::ios::fixed) << std::setprecision(0) 
        << ceil_to_double(dist_b) << endl << ceil_to_double(dist_a) << endl;
    
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


