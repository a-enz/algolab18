#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iomanip>
#include <stdexcept>
#include <limits.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef IK::Point_2						IP;
typedef IK::Segment_2					IS;
typedef IK::Ray_2						IR;

typedef CGAL::Exact_predicates_exact_constructions_kernel EK;
typedef EK::Point_2						EP;
typedef EK::Segment_2					ES;
typedef EK::Ray_2						ER;


double floor_to_double(const EK::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

EP* point_exact(IP point) {
	return new EP(point.x(), point.y());
}

ER* ray_exact(IR ray) {
	IP source = ray.source();
	IP point = ray.point(1); //point(0) is source
	return new ER(*point_exact(source),
				*point_exact(point));
}

ES* segment_exact(IS segment) {
	IP source = segment.source();
	IP target = segment.target();

	return new ES(*point_exact(source),
				*point_exact(target));
}


int main(void) {

	//disable for performance
	ios::sync_with_stdio(false);

	int n_obstacles;
	cin >> n_obstacles;

	while(n_obstacles != 0) {

		vector<IS> obstacles(n_obstacles);
		IR ray_i;
		cin >> ray_i;
		ER* ray_e = ray_exact(ray_i);

		for(int i=0; i<n_obstacles; i++) {
			cin >> obstacles[i];
		}

		//initialize to some big value (seems to work but is questionable)
		//better alternative might be to search first for any intersection to initialize
		//or remember all distances in an array and then use 'min_element' function
		EK::FT sqrd_distance_min = pow(2, 104);
		EP intersection;
		bool do_intersect = false;
		//check all intersections and search minimum
		for(int i=0; i<n_obstacles; i++) {
			if(CGAL::do_intersect(ray_i, obstacles[i])){
				do_intersect = true;
				//compute on exact kernel:
				ES *seg_e = segment_exact(obstacles[i]);
				
				//compute intersection and squared distance
				auto o = CGAL::intersection(*ray_e, *seg_e);
	        	if (const EP* op = boost::get<EP>(&*o)) {
	        		EK::FT dist = CGAL::squared_distance(ray_e->source(),
	        									*op);

	        		if(dist < sqrd_distance_min) {
	        			sqrd_distance_min = dist;
	        			intersection = *op;
	        		}
        		}
	        	else if (const ES* os = boost::get<ES>(&*o)) {
	        		EK::FT dist_s = CGAL::squared_distance(ray_e->source(),
	        									os->source());
	        		EK::FT dist_t = CGAL::squared_distance(ray_e->source(),
	        									os->target());

	        		if(dist_s < sqrd_distance_min) {
	        			sqrd_distance_min = dist_s;
	        			intersection = os->source();
	        		}

	        		if(dist_t < sqrd_distance_min) {
	        			sqrd_distance_min = dist_t;
	        			intersection = os->target();
	        		}
	        	}
	        	else // how could this be? -> error
	          		throw runtime_error("strange segment intersection");
			}
		}

		if(do_intersect){
			cout << setiosflags(ios::fixed) << setprecision(0) << floor_to_double(intersection.x()) <<
				" " << floor_to_double(intersection.y()) << endl;
		}
		else 
			cout << "no\n";
		

		cin >> n_obstacles;
	}

	return 0;
}