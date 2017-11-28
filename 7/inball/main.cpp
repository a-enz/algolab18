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

// round down to next integer double
double floor_to_double(const SolT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

void testcase(int n_ineq, int d) {

    //create the QP objective function
    // by default, we have a nonnegative QP with Ax >= b
    Program lp (CGAL::SMALLER, false, 0, false, 0); 

    // set objective function
    const int r = 0;
    //CGAL always minimizes, therefore invert problem
    lp.set_c(r, -1);

    // set constraints
    for(int ineq=0; ineq<n_ineq; ineq++) {
        int squared_sum = 0;
        for(int j=1; j < d+1; j++) {
            //read constraints
            int a;
            cin >> a;

            lp.set_a(j, ineq, a);
            squared_sum += a*a;
        }

        // constraints on r
        int root = sqrt(squared_sum);
        lp.set_a(r, ineq, root);
        lp.set_l(r, true, 0);

        // b
        int b;
        cin >> b;
        lp.set_b(ineq, b);
    }



    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    // output
    if (s.status() == CGAL::QP_INFEASIBLE) {
        cout << "none\n";
    } 
    else if (s.status() == CGAL::QP_UNBOUNDED){
        cout << "inf\n";
    }
    else {
        assert(s.status() == CGAL::QP_OPTIMAL);
        cout << floor_to_double(-1 * s.objective_value()) << endl;
    }
}

int main(void) {

    //disable for performance
    ios_base::sync_with_stdio(false);
    //read total number of test cases


    //do for each testcase
    int n, d;
    cin >> n >> d;
    while(n != 0) {

        testcase(n, d);
        cin >> n >> d;
    }

    return 0;
}