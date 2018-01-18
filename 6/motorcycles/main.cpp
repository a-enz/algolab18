#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <set>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>


// Namespaces
using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;


struct Biker {
    long y0;
    K::FT slope;
    int idx;
};

K::FT slope(K::FT y0, K::FT y1, K::FT x1) {
    K::FT diff = y1 - y0;
    K::FT x = x1;
    return diff / x;
}

// Functions
// ========= 
void testcases() {
    int n_bikers;
    cin >> n_bikers;
    
    vector<Biker> bikers(n_bikers);
    for (int i = 0; i < n_bikers; i += 1)
    {   
        long y0, x1, y1;
        cin >> y0 >> x1 >> y1;
        bikers[i] = {y0, slope(y0, y1, x1), i};    
    }
    
    
    //sort bikers by y0
    sort(bikers.begin(), bikers.end(), [](const Biker &a, const Biker &b) -> bool {
        return a.y0 > b.y0;
    });
    
    //go through the sorted array and compare neighbors
    set<int> winner;
    Biker prev = bikers[0];
    winner.insert(prev.idx);
    K::FT max_allowed_slope = prev.slope;
    for (unsigned int i = 1; i < n_bikers; i += 1)
    {
        Biker curr = bikers[i];
        K::FT curr_slope = curr.slope;
        if(curr_slope <= max_allowed_slope) {
            winner.insert(curr.idx);
            max_allowed_slope = curr_slope;
        }
        //max_allowed_slope = min(max_allowed_slope, curr_slope);
        prev = curr;
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


