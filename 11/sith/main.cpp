// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

//// Namespaces
using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel     K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>     Vb;
typedef CGAL::Triangulation_face_base_2<K>                      Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>             Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                  Triangulation;
typedef Triangulation::Edge_iterator                            Edge_iterator;

typedef K::Point_2 P;
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
    
    //convert to boost graph
    
    
    
    
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


