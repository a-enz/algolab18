#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iomanip>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Segment_2					S;
typedef K::Ray_2						R;

int main(void) {

	//disable for performance
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);

	int n_obstacles;
	cin >> n_obstacles;

	while(n_obstacles != 0) {

		vector<S> obstacles(n_obstacles);
		R ray;
		cin >> ray;


		for(int i=0; i<n_obstacles; i++) {
			cin >> obstacles[i];
		}

		bool does_intersect=false;
		for(int i=0; i<n_obstacles; i++) {
			if(CGAL::do_intersect(ray, obstacles[i])){
				does_intersect = true;
				break;
			}
		}


		string result = does_intersect ? "yes" : "no";
		cout << result << endl;
		cin >> n_obstacles;
	}

	return 0;
}