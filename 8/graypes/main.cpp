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


void testcase(int n) {

	vector<P> pts(n);

	for(int i=0; i<n; i++) {
		cin >> pts[i];
	}

	//compute triangulation
	Triangulation t;
	t.insert(pts.begin(), pts.end());

	K::FT min_dist = pow(2, 26);
	//for every point, get nearest point
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

	cout << CGAL::sqrt(min_dist) << endl;
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