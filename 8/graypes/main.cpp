#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_iterator	Vertex_iterator;
typedef Triangulation::Vertex_circulator	Vertex_circulator;
typedef K::Point_2	P;

double ceil_to_double(const K::FT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void testcase(int n) {

	vector<P> pts(n);

	for(int i=0; i<n; i++) {
		cin >> pts[i];
	}

	//compute triangulation
	Triangulation t;
	t.insert(pts.begin(), pts.end());

	K::FT min_dist = pow(2, 26);
	// iterate over all vertices and check incident
	// vertices of the triangulation
	for(Vertex_iterator v_it = t.finite_vertices_begin();
		v_it != t.finite_vertices_end();
		v_it++) {


		//Check distance to incident edges
		Vertex_circulator c = t.incident_vertices(v_it);
		do {
			if(not t.is_infinite(c)) {
				K::FT new_dist =
					CGAL::squared_distance(v_it->point(), c->point());
				if(new_dist < min_dist){
					min_dist = new_dist;
				}
			}
		} while(++c != t.incident_vertices(v_it));
	}

	// now that we have shortest distance
	// compute the time it takes for them
	// to meet in the middle, rounded up to next integer
	int time = ceil_to_double(CGAL::sqrt(min_dist) * 50);

	cout << time << endl;
}

int main(void) {

	//disable for performance
	ios_base::sync_with_stdio(false);
	//read total number of test cases
	int n;
	cin >> n;

	//do for each testcase
	while(n != 0) {
		testcase(n);
		cin >> n;
	}

	return 0;
}
