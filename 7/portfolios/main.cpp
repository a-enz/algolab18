#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact rational type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


using namespace std;


void testcase(int n_assets, int m_queries) {

    vector<int> cost(n_assets);
    vector<int> exp_ret(n_assets);
    vector< vector<int> > covariance(n_assets, vector<int>(n_assets));

    //create the QP objective function
    // by default, we have a nonnegative QP with Ax >= b
    Program qp (CGAL::LARGER, true, 0, false, 0); 

    // set sum(a_i*r_i)
    // set sum(a_i*c_i)
    for(int i=0; i<n_assets; i++) {
        cin >> cost[i];
        cin >> exp_ret[i];

        qp.set_a(i, 0, exp_ret[i]);
        qp.set_a(i, 1, cost[i]);
    }


    for(int i=0; i<n_assets; i++) {
        for(int j=0; j<n_assets; j++) {
            cin >> covariance[i][j];

            if(j <= i) {
                qp.set_d(i, j, 2*covariance[i][j]);
            }
        }    
    }


    for(int person=0; person<m_queries; person++) {
        int C, R, V; 
        cin >> C >> R >> V;

        //create the QP constraints for this persons query:
        //sum(a_i*r_i) >= R
        qp.set_b(0, R);

        //sum(a_i*c_i) <= C
        qp.set_b(1, C);
        qp.set_r(1, CGAL::SMALLER);

        // solve the program, using ET as the exact type
        assert(qp.is_nonnegative());
        Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
        assert(s.solves_quadratic_program(qp));

        // output
        if (s.status() == CGAL::QP_INFEASIBLE) {
            cout << "No.\n";
        } else {
            assert (s.status() == CGAL::QP_OPTIMAL);
            if(s.objective_value() <= V) {
                cout << "Yes.\n";
            } else {
                cout << "No.\n";
            }
            
        }
    }
}

int main(void) {

    //disable for performance
    ios_base::sync_with_stdio(false);
    //read total number of test cases


    //do for each testcase
    int n, m;
    cin >> n >> m;
    while(n != 0 || m != 0) {

        testcase(n, m);
        cin >> n >> m;
    }

    return 0;
}