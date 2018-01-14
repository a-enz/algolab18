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
#include <map>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
// choose exact integral type
#include <CGAL/Gmpz.h>

// Namespaces
using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef K::Point_2          P;
// choose exact integral type
typedef CGAL::Gmpz ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// Functions
// ========= 
void testcases() {
    int z_tot, u_tot, v_tot, w_tot;
    cin >> z_tot >> u_tot >> v_tot >> w_tot;

    int n_agents, g_members;
    cin >> n_agents >> g_members;

    vector<P> member_loc(g_members);
    vector<int> u(g_members);
    vector<int> v(g_members);
    vector<int> w(g_members);
    map<P, int> location_idx; //map of: location -> member index
    for (int i = 0; i < g_members; ++i)
    {   
        int x, y;
        cin >> x >> y;
        member_loc[i] = P(x, y);
        location_idx[member_loc[i]] = i;
        cin >> u[i] >> v[i] >> w[i];
    }

    vector<P> agent_loc(n_agents);
    vector<int> z(n_agents);
    for (int i = 0; i < n_agents; ++i)
    {
        int x, y;
        cin >> x >> y;
        agent_loc[i] = P(x, y);
        cin >> z[i];
    }

    //create triangulation of gang members
    Triangulation t;
    t.insert(member_loc.begin(), member_loc.end());

    //go through all agents and find out the closest gang member
    //that costs the least
    map<int, int> assigned_agent; //gang_member -> agent
    for (int i = 0; i < n_agents; ++i)
    {
        P nearest = t.nearest_vertex(agent_loc[i])->point();
        int member_idx = location_idx[nearest];

        auto gm = assigned_agent.find(member_idx);
        if(gm == assigned_agent.end() || 
            z[i] < z[assigned_agent[member_idx]]){ //choose cheaper agent
            
            assigned_agent[member_idx] = i;
        }
    }


    //create LP, variables are hours each agent has to spend observing
    Program lp (CGAL::LARGER, true, 0, true, 24);

    //objective function
    int var = 0;
    for (auto gm : assigned_agent)
    {
        lp.set_c(var++, z[gm.second]);
    }

    //subject to the following constraints
    var = 0;
    for (auto gm : assigned_agent)
    {
        int a_idx = gm.second;
        int g_idx = gm.first;
        lp.set_a(var, 0, u[g_idx]);
        lp.set_a(var, 1, v[g_idx]);
        lp.set_a(var, 2, w[g_idx]);
        var++;        
    }

    lp.set_b(0, u_tot);
    lp.set_b(1, v_tot);
    lp.set_b(2, w_tot);

    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert (s.solves_linear_program(lp));
    // output
    if (s.status() == CGAL::QP_INFEASIBLE) {
        cout << "H\n";
    } else {
        assert (s.status() == CGAL::QP_OPTIMAL);
        if(s.objective_value() > ET(z_tot)) {
            cout << "H\n";
        }
        else 
            cout << "L\n";
    }

}

// Main function looping over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;   // First input line: Number of testcases.
    while(T--)  testcases();
    return 0;
}

