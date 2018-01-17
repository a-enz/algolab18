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
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// Namespaces
using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
// choose exact rational type
typedef CGAL::Gmpz ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


struct CL {
	P center;
	long r_sq;
};




double floor_to_double(const SolT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

// Functions
// ========= 
void testcases() {
	size_t n_warehouse, m_stadiums, c_lines;
	cin >> n_warehouse >> m_stadiums >> c_lines;

	vector<P> warehouse_loc(n_warehouse);
	vector<int> supply(n_warehouse);
	vector<int> alc_perc(n_warehouse);
	for (int i = 0; i < n_warehouse; ++i)
	{
		int x, y, s, a;
		cin >> x >> y >> s >> a;

		warehouse_loc[i] = P(x, y);
		supply[i] = s;
		alc_perc[i] = a;
	}

	vector<P> stadium_loc(m_stadiums);
	vector<int> demand(m_stadiums);
	std::vector<int> limit(m_stadiums);
	for (int i = 0; i < m_stadiums; ++i)
	{
		int x, y, d, u;
		cin >> x >> y >> d >> u;
		stadium_loc[i] = P(x, y);
		demand[i] = d;
		limit[i] = u;
	}

	vector< vector<int> > revenue(n_warehouse, vector<int>(m_stadiums));
	for (int w = 0; w < n_warehouse; ++w)
	{
		for (int s = 0; s < m_stadiums; ++s)
		{
			cin >> revenue[w][s];
			revenue[w][s] *= 100;
		}
	}

	vector<CL> clines(c_lines);
	for (int i = 0; i < c_lines; ++i)
	{
		int x, y;
		long r;
		cin >> x >> y >> r;
		clines[i] = {P(x, y), r * r};
	}

	//sort contour lines by radius descending
	std::sort(clines.begin(), clines.end(), [](const CL& a, const CL& b) -> bool {
		return a.r_sq > b.r_sq;
	});

	//compute for each warehouse and stadium the circles it is contained in
	vector< vector<int> > w_circles(n_warehouse);
	vector< vector<int> > s_circles(m_stadiums);

	for (int c = 0; c < c_lines; ++c)
	{	
		CL *cl = &clines[c];
		for (int w = 0; w < n_warehouse; ++w)
		{
			K::FT dd = CGAL::squared_distance(warehouse_loc[w], cl->center);
			if(dd <= cl->r_sq)
				w_circles[w].push_back(c);
		}

		for (int s = 0; s < m_stadiums; ++s)
		{
			K::FT dd = CGAL::squared_distance(stadium_loc[s], cl->center);
			if(dd <= cl->r_sq)
				s_circles[s].push_back(c);
		}
	}

	//update revenue
	for (int w = 0; w < n_warehouse; ++w)
	{
		for (int s = 0; s < m_stadiums; ++s)
		{
			int e = 0;
			//compare containing circles until they differ
			//since have been added in sorted order this is correct 
			while(e < w_circles[w].size() &&
					e < s_circles[s].size() &&
					w_circles[w][e] == s_circles[s][e])
				e++;

			revenue[w][s] -= w_circles[w].size() + s_circles[s].size() - 2*e;
		}
	}

	//create LP (minimizes, therefore need to invert obj. function)
	Program lp (CGAL::SMALLER, true, 0, false, 0); 

	//variables are l_w_s which means the index is = w * m_stadiums + s
	//objective function and warehouse constraints
	for (int w = 0; w < n_warehouse; ++w)
	{
		for (int s = 0; s < m_stadiums; ++s)
		{	
			int l = w * m_stadiums + s;
			lp.set_c(l, -1 * revenue[w][s]); //objective function
			lp.set_a(l, w, 1); //supply 
			lp.set_a(l, s + n_warehouse, 1); //demand
			lp.set_a(l, s + n_warehouse + m_stadiums, alc_perc[w]); //alcohol limit
		}

		lp.set_b(w, supply[w]);
		lp.set_r(w, CGAL::SMALLER);
	}

	for (int s = 0; s < m_stadiums; ++s)
	{
		lp.set_b(s + n_warehouse, demand[s]);
		lp.set_r(s + n_warehouse, CGAL::EQUAL); //demand

		lp.set_b(s + n_warehouse + m_stadiums, 100 * limit[s]); //alcohol limit
		lp.set_r(s + n_warehouse + m_stadiums, CGAL::SMALLER);
	}

	// solve the program, using ET as the exact type
	Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
	assert (s.solves_nonnegative_linear_program(lp));

	if(s.is_optimal()) {
		cout << std::setiosflags(std::ios::fixed) << std::setprecision(0) 
	    	<< floor_to_double(s.objective_value() / -100) << endl;;
	}
	else 
		cout << "RIOT!\n";
	
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}

