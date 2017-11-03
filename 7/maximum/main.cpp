#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double ceil_to_double(const CGAL::Quotient<ET>& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void do_test() {
	int type;
	cin >> type;

	const int X=0;
	const int Y=1;
	const int Z_prim=2;

	while(type != 0) {

		int a, b;
		cin >> a >> b;

		Solution s;

		if(type == 1) {
			Program qp(CGAL::SMALLER, true, 0, false, 0);

			//minimize ax² - by
			qp.set_d(X, X, 2*a);
			qp.set_c(Y, -b);

			//x + y <= 4
			qp.set_a(X, 0, 1); qp.set_a(Y, 0, 1); qp.set_b(0, 4);
			//4x + 2y <= ab
			qp.set_a(X, 1, 4); qp.set_a(Y, 1, 2); qp.set_b(1, a*b);
			//-x + y <= 1
			qp.set_a(X, 2, -1); qp.set_a(Y, 2, 1); qp.set_b(2, 1);

			// solve the program, using ET as the exact type
		  	s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
			assert (s.solves_quadratic_program(qp));
		}
		else if(type == 2) {
			Program qp(CGAL::LARGER, false, 0, false, 0);

			//minimize ax² + by + z'^2 (z' := z^2)
			qp.set_d(X, X, 2*a);
			qp.set_d(Z_prim, Z_prim, 2);
			qp.set_c(Y, b);

			//x, y <= 0
			qp.set_u(X, true, 0);
			qp.set_u(Y, true, 0);

			//z' >= 0
			qp.set_l(Z_prim, true, 0);

			//x + y >= -4
			qp.set_a(X, 0, 1); qp.set_a(Y, 0, 1); qp.set_b(0, -4);
			//4x + 2y + z' >= -ab
			qp.set_a(X, 1, 4); qp.set_a(Y, 1, 2); qp.set_a(Z_prim, 1, 1); qp.set_b(1, -(a*b));
			//-x + y >= -1
			qp.set_a(X, 2, -1); qp.set_a(Y, 2, 1); qp.set_b(2, -1);

			// solve the program, using ET as the exact type
		  	s = CGAL::solve_quadratic_program(qp, ET());
			assert (s.solves_quadratic_program(qp));
		}
		else {
			cerr << "Unknown problem type \"" << type << "\"" << endl;
			exit(1);
		}

		if (s.status() == CGAL::QP_INFEASIBLE) {
			cout << "no\n";
		}
		else if(s.status() == CGAL::QP_UNBOUNDED) {
			cout << "unbounded\n";
		}
		else {

			double sol = ceil_to_double(s.objective_value());
			if(type == 1) {
				cout << -1*sol << endl;
			}
			else {//type == 2
				cout << setiosflags(ios::fixed) << setprecision(0) <<
					sol << endl;
			}
		}

		cin >> type;
	}
}

int main(void) {

	//disable for performance
	ios_base::sync_with_stdio(false);
	//read total number of test cases


	//do for each testcase

	do_test();

	return 0;
}