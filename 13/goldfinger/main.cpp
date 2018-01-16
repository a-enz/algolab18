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
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// Namespaces
using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef K::Point_2      P; 

// choose exact rational type
typedef CGAL::Gmpq ET;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


//global variables
int n_sensors, I_max;
vector<P> sensor_loc, mpe_loc;
vector<K::FT> mpe_maxrad;
vector<int> sensor_de;

bool is_impossible(int mid) {
    //set up LP
    Program lp (CGAL::LARGER, true, 0, false, 0);

    //set up conditions
    for (int s = 0; s < n_sensors; ++s)
    {
        //sensor must receive a certain energy from mpes
        for (int k = 0; k < mid; ++k)
         {
            K::FT dd = CGAL::squared_distance(sensor_loc[s], mpe_loc[k]);
            ET val;
            if(dd < mpe_maxrad[k]) {
                val = ET(1) / ET(dd);
            }
            else {
                val = ET(0);
            }

            lp.set_a(k, s, val);
         }

        lp.set_b(s, sensor_de[s]);
    }

    // total amount of energy spent cannot be larger than I_max
    for (int k = 0; k < mid; ++k)
    {
        lp.set_a(k, n_sensors, -1);
    }

    lp.set_b(n_sensors, -I_max);

    //solve lp
    assert(lp.is_nonnegative());
    Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    return s.status() == CGAL::QP_INFEASIBLE;
}

// Functions
// ========= 
void testcases() { 
    int m_mpe, h_men;
    cin >> n_sensors >> m_mpe >> h_men >> I_max;
    
    sensor_loc = vector<P>(n_sensors);
    sensor_de = vector<int>(n_sensors);
    mpe_loc = vector<P>(m_mpe);
    mpe_maxrad = vector<K::FT>(m_mpe, K::FT(LONG_MAX));
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
        
    if(h_men != 0) {
        for (unsigned int i = 0; i < m_mpe; i += 1)
        {
            P nearest = t.nearest_vertex(mpe_loc[i])->point();
            K::FT d = CGAL::squared_distance(nearest, mpe_loc[i]);
            mpe_maxrad[i] = d;
        }  
    } 

    //check if it is possible with all mpes
    if(is_impossible(m_mpe)) {
        cout << "impossible\n";
        return;
    }

    //do binary search on number of triggered shots
    int lmin = 0, lmax = m_mpe;
    while(lmin != lmax) {
        int mid = lmin + (lmax - lmin) / 2;

        if(is_impossible(mid))
            lmin = mid+1;
        else 
            lmax = mid;
    }

    cout << lmin << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


