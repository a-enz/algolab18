#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <set>
#include <stack>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

// Namespaces
using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Ray_2 R;
typedef K::Point_2 P;

struct Biker {
    R ray;
    int idx;
};

// Functions
// ========= 
void testcases() {
    int n_bikers;
    cin >> n_bikers;
    
    vector<Biker> bikers(n_bikers);
    for (int i = 0; i < n_bikers; i += 1)
    {   
        double y0, x, y1;
        cin >> y0 >> x >> y1;  
        bikers[i] = {R(P(0, y0), P(x, y1)), i};
    }
    
    //sort bikers ascending by y0
    sort(bikers.begin(), bikers.end(), [](const Biker &a, const Biker &b) -> bool {
        return a.ray.source().y() < b.ray.source().y();
    });

    stack<Biker> picked;
    picked.push(bikers[0]);

    for (int i = 1; i < n_bikers; ++i)
    {
        //compare this biker to already picked bikers
        //and decide which to discard or keep
        Biker current = bikers[i];
        Biker prev = picked.top();
        if(not CGAL::do_intersect(current.ray, prev.ray)) {
            //bikers run in parallel or away from each other
            picked.push(current);
        }
        else if(prev.ray.direction().dy() > 0) { //previous ray goes upwards
            if(current.ray.direction().dy() >= 0) { //current ray goes horizontal or up
                //remove top of stack until no intersection
                while(not picked.empty() && 
                        CGAL::do_intersect(current.ray, picked.top().ray)) {
                    picked.pop();
                }
                picked.push(current);
            }
            else { //current ray goes downwards
                assert(current.ray.direction().dy() < 0);

                auto dir = current.ray.direction();
                auto src = current.ray.source();
                R mirrored = R(src, P(dir.dx(), src.y() - dir.dy()));

                while(not picked.empty() && CGAL::do_intersect(mirrored, picked.top().ray)) {
                    picked.pop();
                }

                if(picked.empty() || not CGAL::do_intersect(current.ray, picked.top().ray)) {
                    picked.push(current);  
                }
            }
        }
        //else discard 'current' if it intersects with 'prev'
        //  and prev is not going upwards
    }

    //put winners into ordered set
    set<int> winner;
    while(not picked.empty()) {
        Biker b = picked.top();
        picked.pop();
        winner.insert(b.idx);
    }
    
    for (int w : winner)
    {
        cout << w << " ";
    }
    
    cout << endl;
    
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


