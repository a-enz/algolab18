#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;
typedef K::Point_2  P;


void testcase(int n_bistros) {

    //read existing bistro locations
    vector<P> pts(n_bistros);
    for (int i = 0; i < n_bistros; ++i)
    {
        cin >> pts[i]; 
    }

    //read new bistro locations
    int m_queries;
    cin >> m_queries;
    vector<P> new_pts(m_queries);

    for (int i = 0; i < m_queries; ++i)
    {
        cin >> new_pts[i];
    }

    // Triangulation of existing bistros:
    Triangulation t;
    t.insert(pts.begin(), pts.end());


    for(vector<P>::iterator p_it = new_pts.begin();
        p_it != new_pts.end();
        ++p_it
        )
    {
        P q = t.nearest_vertex(*p_it)->point();

        cout << setiosflags(ios::fixed) << setprecision(0) <<
            CGAL::squared_distance(*p_it, q) << endl;
    }
}

int main(void) {

    //disable for performance
    ios_base::sync_with_stdio(false);
    //read total number of test cases
    int n;
    cin >> n;

    //do for each testcase
    while(n != 0){
        testcase(n);
        cin >> n;
    }

    return 0;
}