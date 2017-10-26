#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iomanip>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef  CGAL::Min_circle_2_traits_2<IK>  Traits_inex;
typedef  CGAL::Min_circle_2<Traits_inex>      Min_circle_inex;
typedef Min_circle_inex::Support_point_iterator Sup_point_it_inex;
typedef  CGAL::Min_circle_2_traits_2<EK>  Traits_ex;
typedef  CGAL::Min_circle_2<Traits_ex>      Min_circle_ex;
typedef IK::Point_2						 P_inex;
typedef EK::Point_2						P_ex;

double ceil_to_double(const EK::FT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
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

		Min_circle_inex mc(coordinates.begin(), coordinates.end(), true);

		//now transfer this into an exact solution by using support points
		vector<P_ex> support_points;

		for(Sup_point_it_inex it = mc.support_points_begin();
			it != mc.support_points_end();
			it++) {

			P_ex p(it->x(), it->y());
			//cout << "supp point: " << p << endl;
			support_points.push_back(p);
		}

		Min_circle_ex mc_ex(support_points.begin(), support_points.end(), true);
		Traits_ex::Circle c = mc_ex.circle();




  		EK::FT d = CGAL::sqrt(c.squared_radius());

  		//cout << "not rounded " << setiosflags(ios::fixed) << setprecision(5) << d << " rounded: ";
		cout << setiosflags(ios::fixed) << setprecision(0) << ceil_to_double(d) << endl;


		cin >> n_persons;
	}

	return 0;
}