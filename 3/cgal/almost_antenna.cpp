#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
//#include <iomanip>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef  CGAL::Min_circle_2_traits_2<IK>  Traits_inex;
typedef  CGAL::Min_circle_2<Traits_inex>      Min_circle_inex;
typedef Min_circle_inex::Support_point_iterator Sup_point_it_inex;
typedef  CGAL::Min_circle_2_traits_2<EK>  Traits_ex;
typedef  CGAL::Min_circle_2<Traits_ex>      Min_circle_ex;
typedef Min_circle_ex::Support_point_iterator Sup_point_it_ex;
typedef IK::Point_2						 P_inex;
typedef EK::Point_2						P_ex;

double ceil_to_double(const EK::FT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}


Min_circle_ex *exact_mc_from_sup(Sup_point_it_inex start,
								Sup_point_it_inex end) {

	vector<P_ex> support_points;
	for(Sup_point_it_inex it = start;
		it != end;
		it++) {

		P_ex p(it->x(), it->y());
		support_points.push_back(p);
	}

	return new Min_circle_ex(support_points.begin(), support_points.end(), true);
}


int main(void) {

	//disable for performance
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);

	int n_persons;
	cin >> n_persons;

	while(n_persons != 0) {

		vector<P_inex> coordinates(n_persons);

		for(int i=0; i < n_persons; i++) {
			cin >> coordinates[i];
		}

		if(n_persons <= 2) {
			//we already know the answer
			cout << 0 << endl;
			cin >> n_persons;
			continue;
		}

		Min_circle_inex mc(coordinates.begin(), coordinates.end(), true);

  		int n_support_points = mc.number_of_support_points();
  		//only from now on use exact kernel

  		EK::FT r = 0;
  		if(n_support_points >= 2) {

  			Min_circle_ex *mc_ex = exact_mc_from_sup(mc.support_points_begin(), 
  							mc.support_points_end());

  			//iterate over points and return best mc
  			r = mc_ex->circle().squared_radius();

  			for(Sup_point_it_ex it = mc_ex->support_points_begin();
  				it != mc_ex->support_points_end();
  				it++) {

  				vector<P_inex> remaining_coord(n_persons-1);

  				int i=0;
  				//cout << "thus testing on points ";
  				for(vector<P_inex>::iterator p_it = coordinates.begin();
  					p_it != coordinates.end(); 
  					p_it++) {

  					if(p_it->x() != it->x() ||
  						p_it->y() != it->y()) {
  						remaining_coord[i] = *p_it;
  						i++;
  					}
  				}

				Min_circle_inex remaining_mc(remaining_coord.begin(),remaining_coord.end(), true);

				Min_circle_ex *remaining_mc_ex = exact_mc_from_sup(remaining_mc.support_points_begin(), 
  								remaining_mc.support_points_end());

				EK::FT r_candidate_new = remaining_mc_ex->circle().squared_radius();
				
				if(r > r_candidate_new) {
					r = r_candidate_new;
				}
  			}
  		}

  		EK::FT result = CGAL::sqrt(r);

		cout << setiosflags(ios::fixed) << setprecision(0) << ceil_to_double(result) << endl;

		cin >> n_persons;
	}

	return 0;
}