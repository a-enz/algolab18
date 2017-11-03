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

double floor_to_double(const CGAL::Quotient<ET>& x)
{
	double a = std::floor(CGAL::to_double(x));
		while (a > x) a -= 1;
		while (a+1 <= x) a += 1;
	return a;
}

void do_test() {
	int n, m;
	cin >> n >> m;

	while(not (n==0 && m==0)) {

		//read nutrition bounds
		vector<int> max(n);
		vector<int> min(n);
		for(int i=0; i<n; i++) {
			cin >> min[i];
			cin >> max[i];
		}

		//read product values
		vector<int> price(m);
		vector< vector<int> > amount(m, vector<int>(n));
		for(int j=0; j<m; j++) {
			cin >> price[j];
			for(int i=0; i<n; i++) {
				cin >> amount[j][i];
			}
		}

		//construct LP
		Program lp (CGAL::SMALLER, true, 0, false, 0);

		//min function
		for(int j=0; j<m; j++) {
			lp.set_c(j, price[j]);
		}

		//set constrictions
		for(int i=0; i<n; i++) {
			for(int j=0; j<m; j++) {
				//for max_i condition
				lp.set_a(j, i, amount[j][i]);
				//for min_i condition
				lp.set_a(j, i+n, -(amount[j][i]));
			}
			lp.set_b(i, max[i]);
			lp.set_b(i+n, -(min[i]));
		}

		// solve the program, using ET as the exact type
		Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
		assert (s.solves_linear_program(lp));


		if (s.status() == CGAL::QP_INFEASIBLE) {
			cout << "No such diet.\n";
		}
		else if(s.status() == CGAL::QP_UNBOUNDED) {
			cerr << "Unbounded LP not expected\n";
			exit(1);
		}
		else {
			cout << setiosflags(ios::fixed) << setprecision(0) <<
					floor_to_double(s.objective_value()) << endl;
			
		}

		cin >> n >> m;
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