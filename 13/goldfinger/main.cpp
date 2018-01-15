// ALGOLAB BGL Tutorial 1
// Tutorial example problem

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl-tutorial_problem.cpp -o bgl-tutorial_problem; ./bgl-tutorial_problem < bgl-tutorial_problem.in
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl-tutorial_problem.cpp -o bgl-tutorial_problem; ./bgl-tutorial_problem < bgl-tutorial_problem.in

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

// Namespaces
using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef K::Point_2      P; 


// Functions
// ========= 
void testcases() {
    int n_sensors, m_mpe, h_men, I_max;
    cin >> n_sensors >> m_mpe >> h_men >> I_max;
    
    vector<P> sensor_loc(n_sensors);
    vector<int> sensor_de(n_sensors);
    vector<P> mpe_loc(m_mpe);
    vector<K::FT> mpe_maxrad(m_mpe);
    vector<P> hench_loc(h_men);
    
    for (unsigned int i = 0; i < n_sensors; i += 1)
    {
        int x, y;
        cin >> x >> y;
        sensor_loc[i] = P(x, y);
        
        cin >> sensor_de[i];
    }
    
    for (unsigned int i = 0; i < m_mpe; i += 1)
    {
        int x, y;
        cin >> x >> y;
        mpe_loc[i] = P(x, y);
    }
    
    for (unsigned int i = 0; i < h_men; i += 1)
    {
        int x, y;
        cin >> x >> y;
        hench_loc[i] = P(x, y);
    }
    
    
    // Henchmen triangulation and maximum radii computation
    Triangulation t;
    t.insert(hench_loc.begin(), hench_loc.end());
    
    for (unsigned int i = 0; i < m_mpe; i += 1)
    {
        P nearest = t.nearest_vertex(mpe_loc[i])->point();
        K::FT d = CGAL::squared_distance(nearest, mpe_loc[i]);
        mpe_maxrad[i] = d;
    }   

}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


