#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include "prettyprint.hpp"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;

using namespace std;

//global variables
vector< vector<int> > triangle_coverage;
vector<P> pts;


bool is_inside(const vector<P>& sides, const P& candidate) {
    //check all 3 sides
    assert(sides.size() == 6);
    for (int i = 0; i < 6; i+=2)
    {
        if(CGAL::right_turn(sides[i], sides[i+1], candidate))
            return false;
    }
    return true;
}

int update_coverage(const std::vector<int>& covered_paths , vector<int>& counts, const int update) {
    assert(update == 1 || update == -1);
    int amount_new_covered = 0;

    for (int i = 0; i < covered_paths.size(); ++i)
    {   
        int path_idx = covered_paths[i];
        if(update == 1 && counts[path_idx] == 0)
            amount_new_covered += 1;

        counts[path_idx] += update;

        if(update == -1 && counts[path_idx] == 0)
            amount_new_covered -= 1;

        assert(counts[path_idx] >= 0);
    }

    return amount_new_covered;
}

void testcase() {
	size_t m_points, n_triangles;
	cin >> m_points >> n_triangles;

    pts = vector<P>(m_points);
    for (int i = 0; i < m_points; ++i)
    {
        int x, y;
        cin >> x >> y;
        pts[i] = P(x, y);
    }

    triangle_coverage = vector< vector<int> >(n_triangles, vector<int>());
    for (int i = 0; i < n_triangles; ++i)
    {   
        vector<P> tr;
        for (int q = 0; q < 6; ++q)
        {   
            int x, y;
            cin >> x >> y;
            tr.push_back(P(x, y));
        }

        //ensure correct orientation of line for a given triangle
        //"outside" of the triangle should be a 'right turn'
        //which means the "inside" (collinear and left_turn)
        //can be checked quickly with "not right_turn"
        for (int p = 0; p < 6; p += 2)
        {
            if(CGAL::right_turn(tr[p], tr[p+1], tr[(p+2) % 6])) {
                std::swap(tr[p], tr[p+1]);
            }
        }

        //check which paths this triangle covers
        bool prev = is_inside(tr, pts[0]);
        for (int p = 1; p < m_points; ++p)
        {
            bool next = is_inside(tr, pts[p]);
            if(prev && next) {
                triangle_coverage[i].push_back(p-1);
            }
            prev = next;
        }
    }

    //do sliding window over the sequence of triangles
    int left = 0, right = 0;
    //keep track of how many triangles cover a leg 
    vector<int> covered(m_points-1, 0);

    //initialize with the first triangle
    int total_covered = update_coverage(triangle_coverage[0], covered, 1);
    int min_cost = n_triangles;    

    if(total_covered == m_points-1) {
        cout << "1\n";
        return;
    }

    while(left < n_triangles) {
        assert(total_covered >= 0 && total_covered <= m_points-1);
        assert(left <= right);

        int diff;
        if(right < n_triangles-1 && total_covered < m_points-1) {
            //increase sequence length
            right++;
            //update coverage
            diff = update_coverage(triangle_coverage[right], covered, 1);
        }
        else {
            diff = update_coverage(triangle_coverage[left], covered, -1);
            left++;
        }

        total_covered += diff;
        if(total_covered == m_points-1) {
            int cost = right - left + 1;
            min_cost = min(cost, min_cost);
            if(left == right)
                break;
        } 
        else if(total_covered < m_points-1 && right == n_triangles-1)
            break;
    }

	//write output values for testcase
	cout << min_cost << endl;
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