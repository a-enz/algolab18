#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
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

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2	P;

using namespace std;

void testcase() {
	int a, s, b;
	cin >> a >> s >> b;

	int e;
	cin >> e;

	//read locations of a, s, b
	vector<P> a_loc(a);
	vector<int> a_en(a);
	for (int i = 0; i < a; ++i)
	{
		cin >> a_loc[i];
		cin >> a_en[i];
	}

	vector<P> s_loc(s);
	for (int i = 0; i < s; ++i)
	{
		cin >> s_loc[i];
	}


	vector<P> b_loc(b);
	for (int i = 0; i < b; ++i)
	{
		cin >> b_loc[i];
	}

	vector<K::FT> max_sq_radius(s);
	if(b != 0) { //care! there can be zero bounty hunters
		//Triangulation of bounthunters
		Triangulation t;
	  	t.insert(b_loc.begin(), b_loc.end());

	  	//for each shot, compute maximally allowed radius
	  	//by checking the triangulation
	  	for (int i = 0; i < s; ++i)
	  	{
	  		P nearest = t.nearest_vertex(s_loc[i])->point();
	  		max_sq_radius[i] = CGAL::squared_distance(s_loc[i], nearest);	
	  	}
  	} else {
  		// basically allows all (integer) radii of dist(a,s)
  		max_sq_radius = vector<K::FT>(s, INT_MAX); 
  	}


 	//create the LP
  	Program lp (CGAL::LARGER, true, 0, false, 0);

  	//minimization function
  	for (int i = 0; i < s; ++i)
  	{
  		lp.set_c(i, 1);
  	}

  	//constricted by:
  	for (int i = 0; i < a; ++i)
  	{
  		for (int j = 0; j < s; ++j)
  		{

  			K::FT dist = CGAL::squared_distance(s_loc[j], a_loc[i]);

  			double d_inv = 0;

  			//check if a_loc[i] is inside the blast radius of s_loc [j]
  			if(dist < max_sq_radius[j]) {
  				//compute proper distance between a and s
  				if(dist == 0) {
  					d_inv = 1;
  				}
  				else {
  					d_inv = 1 / CGAL::to_double(dist);	
  				}
  			}

  			lp.set_a(j, i, d_inv);
  		}

  		lp.set_b(i, a_en[i]);
  	}

	 // solve the program, using ET as the exact type
	Solution sol = CGAL::solve_linear_program(lp, ET());
	assert (sol.solves_linear_program(lp));

	// output
	if (sol.status() == CGAL::QP_INFEASIBLE) {
		cout << "n\n";
	} else {
		assert (sol.status() == CGAL::QP_OPTIMAL);

		//check if we are below allowed energy

		SolT min_energy = sol.objective_value();
		if(min_energy > e) {
			cout << "n\n";
		}
		else {
			cout << "y\n";
		}
	}






}

int main(void) {

	//disable for performance
	ios_base::sync_with_stdio(false);
	//read total number of test cases
	int n_total_testcases;
	cin >> n_total_testcases;

	//do for each testcase
	for(int n_test = 0; n_test < n_total_testcases; n_test++) {
		testcase();
	}

	return 0;
}