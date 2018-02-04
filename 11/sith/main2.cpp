#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <climits>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>    Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Triangulation;
typedef K::Point_2  P;

typedef Triangulation::Edge_iterator  Edge_iterator;
typedef boost::disjoint_sets_with_storage<>    DS;

//global variables
int n_planets;
    
 
int max_comp_size(DS& components) {
    
    vector<int> sizes(n_planets, 0); 
    for (int i = 0; i < n_planets; i += 1)
    {   
        int set_n=components.find_set(i);

        assert(set_n < n_planets);
        sizes[set_n]++;   
    }
    return *max_element(sizes.begin(), sizes.end());
}

bool too_small(int k, const vector< pair<P, int> >& planets, double rr) {

    Triangulation t;
    t.insert(planets.begin() + k, planets.end());
    
    DS components(n_planets);
    
    //find all components by doing union of planets that are in range of each other
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        if(t.segment(e).squared_length() <= rr) {
            Triangulation::Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
            Triangulation::Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
        
            components.union_set(v1->info(), v2->info());
        }
    }
    
    //now compute largest components
    return k <= max_comp_size(components);
}


void testcases() {
    double rr;
    cin >> n_planets >> rr;
    rr = rr * rr;
    
    vector< pair<P, int> > planets(n_planets);
    for (int i = 0; i < n_planets; i += 1)
    {
        int x, y;
        cin >> x >> y;
        planets[i] = {P(x, y), i};
    }
    
    
    int lmin = 1, lmax = n_planets / 2 + 1;
    
    while(lmin < lmax) {
        int mid = lmin + (lmax - lmin) / 2;

        if(too_small(mid, planets, rr))
            lmin = mid+1; //find first element not too_small
        else
            lmax = mid;
    }
    
    //want last element that is too_small 
    cout << lmin-1 << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


