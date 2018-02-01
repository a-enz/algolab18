#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <cmath>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Gmpq ET;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef K::Point_2  P;

// Namespaces
using namespace std;

//global variables
const double INF_DOUBLE = numeric_limits<double>::max();
int I_max;
vector<P> sensor_pos;
vector<int> sensor_nrg;
vector<P> mpe_loc;

bool too_small(int k, const vector<K::FT>& max_r) {
    //construct LP and check it is possible to destroy the
    //sensors using the k first mpe
    Program lp (CGAL::SMALLER, false, 0, false, 0);

    assert(k <= mpe_loc.size());
    //minimum energy constraints for each sensor
    for (int i = 0; i < sensor_pos.size(); ++i)
    {
        for (int m = 0; m < k; ++m)
        {
            //if the mpe reaches this sensor
            K::FT sq_dist = CGAL::squared_distance(mpe_loc[m], sensor_pos[i]);
            if(sq_dist < max_r[m])
                lp.set_a(m, i, 1 / ET(sq_dist));
            else
                lp.set_a(m, i, 0);
        }

        lp.set_b(i, sensor_nrg[i]);
        lp.set_r(i, CGAL::LARGER); // override default
    }

    //one additional constraint for the sum of total energies
    int cn = sensor_pos.size();
    for (int i = 0; i < k; ++i)
    {
        lp.set_a(i, cn, 1);
    }
    lp.set_b(cn, I_max);
    lp.set_r(cn, CGAL::SMALLER); // override default


    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
    assert (s.solves_nonnegative_linear_program(lp));

    return s.is_infeasible();
}

// Functions
// ========= 
void testcases() {
    int n_sensors, m_mpe, h_men;
    cin >> n_sensors >> m_mpe >> h_men >> I_max;

    sensor_pos = vector<P>(n_sensors);
    sensor_nrg = vector<int>(n_sensors);
    for (int i = 0; i < n_sensors; ++i)
    {   
        int x, y, e;
        cin >> x >> y >> e;
        sensor_pos[i] = P(x, y);
        sensor_nrg[i] = e;
    }

    mpe_loc= vector<P>(m_mpe);
    for (int i = 0; i < m_mpe; ++i)
    {
        int x, y;
        cin >> x >> y;
        mpe_loc[i] = P(x, y);
    }

    vector<P> h_loc(h_men);
    for (int i = 0; i < h_men; ++i)
    {
        int x, y;
        cin >> x >> y;
        h_loc[i] = P(x, y);
    }

    //Find out the maximum allowed squared radius for each mpe considering
    //nearby henchmen
    Triangulation t;
    t.insert(h_loc.begin(), h_loc.end());

    vector<K::FT> max_radius(m_mpe, INF_DOUBLE);

    if(h_men > 0) {
        for (int i = 0; i < m_mpe; ++i)
        {
            P nearest = t.nearest_vertex(mpe_loc[i])->point();
            max_radius[i] = CGAL::squared_distance(mpe_loc[i], nearest);
        }
    }


    //do binary search on the minimum number of activated MPE
    int lmin = 1, lmax = 1;
    while(lmax <= m_mpe && too_small(lmax, max_radius)) {
        lmin = lmax;
        lmax *= 2;
    }

    //check if it is impossible even with all mpe
    if(lmax > m_mpe && too_small(m_mpe, max_radius)) {
        cout << "impossible\n";
        return;
    }

    //clip lmax
    lmax = min(lmax, m_mpe);

    while(lmin < lmax) {
        int mid = (lmin + lmax) / 2;

        if(too_small(mid, max_radius))
            lmin = mid+1;
        else
            lmax = mid;
    }
    
    cout << lmin << endl;
}

// Main function looping over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;   // First input line: Number of testcases.
    while(T--)  testcases();
    return 0;
}

