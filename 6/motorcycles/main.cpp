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
    long long y0, x1, y1;
    int idx;
};

K::FT slope(const Biker& b) {
    K::FT diff = b.y1 - b.y0;
    K::FT x = b.x1;
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
        bikers[i] = {y0, x1, y1, i};    
    }
    
    
    //sort bikers by y0
    sort(bikers.begin(), bikers.end(), [](const Biker &a, const Biker &b) -> bool {
        return a.y0 > b.y0;
    });
    
    //go through the sorted array and compare neighbors
    set<int> winner;
    Biker prev = *bikers.begin();
    winner.insert(prev.idx);
    K::FT max_allowed_slope = slope(prev);
    for (unsigned int i = 1; i < n_bikers; i += 1)
    {
        Biker curr = *next(bikers.begin(), i);
        K::FT curr_slope = slope(curr);
        if(curr_slope <= max_allowed_slope) {
            winner.insert(curr.idx);
        }
        max_allowed_slope = min(max_allowed_slope, curr_slope);
        prev = curr;
    }
    
    for (unsigned int i = 0; i < winner.size(); i += 1)
    {
        cout << *next(winner.begin(), i) << " ";
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


