#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include "prettyprint.hpp"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef K::Point_2  P;
// Namespaces
using namespace std;

// Functions
// ========= 

double ceil_to_double(const EK::FT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

EK::FT time(const EK::FT& sq_dist) {
    return CGAL::sqrt((CGAL::sqrt(sq_dist) - 1) / 2);
}

void testcases(const int n_bact) {

    int left, bottom, right, top;
    cin >> left >> bottom >> right >> top;

    vector< pair<P, int> > bact(n_bact);
    for (int i = 0; i < n_bact; ++i)
    {
        int x, y;
        cin >> x >> y;
        bact[i] = make_pair(P(x, y), i);
    }

    //compute all the distances of bacteria to possible collision objects.
    //this means for each bacteria b:
    // - compute distance to all four borders
    // - compute distance to all neighbors in triangulation of bacteria

    //for each bacteria, remember closest object (will collide with it first)
    vector<K::FT> collision_dist(n_bact);
    for (int i = 0; i < n_bact; ++i)
    {
        K::FT d_top, d_left, d_bottom, d_right;
        d_top = top - bact[i].first.y();
        d_top *= 4*d_top;

        d_left = bact[i].first.x() - left;
        d_left *= 4*d_left;

        d_bottom = bact[i].first.y() - bottom;
        d_bottom *= 4*d_bottom;

        d_right = right - bact[i].first.x();
        d_right *= 4*d_right;

        collision_dist[i] = min({d_top, d_left, d_bottom, d_right});
    }


    //now do the triangulation
    Triangulation t;
    t.insert(bact.begin(), bact.end());

    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e){
        // vertices of e
        Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
        Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);

        K::FT dist = CGAL::squared_distance(v1->point(), v2->point());
        //update v1 and v2
        collision_dist.at(v1->info()) = min(collision_dist.at(v1->info()), dist);
        collision_dist.at(v2->info()) = min(collision_dist.at(v2->info()), dist);
    }

    sort(collision_dist.begin(), collision_dist.end());

    int middle = n_bact / 2;


    cout << std::setiosflags(std::ios::fixed) << std::setprecision(0) 
        << ceil_to_double(time(collision_dist.front())) << " "
        << ceil_to_double(time(collision_dist[middle])) << " "
        << ceil_to_double(time(collision_dist.back())) << std::endl;

}

// Main function looping over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;   // First input line: Number of testcases.
    while(T != 0)  {
        testcases(T);
        cin >> T;
    }
    return 0;
}

