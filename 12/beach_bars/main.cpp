// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <assert.h>

// Namespaces
using namespace std;

// Functions
// ========= 
void testcases() {
    size_t n;
    cin >> n;
    
    //read input
    vector<int> loc(n);
    for (size_t i = 0; i < n; i += 1)
    {
        cin >> loc[i];
    }
    
    //sort by location
    sort(loc.begin(), loc.end());
    
    //compute sliding window over sorted locations
    vector< pair<int, int> > border_indices;
    
    //'sliding window' by increasing left border and doing binary search
    // on the right border
    for (size_t low = 0; low < n; low += 1)
    {
        auto up = upper_bound(loc.begin(), loc.end(), loc[low]+200);
        up--;
        
        int high = up - loc.begin();

        border_indices.push_back(make_pair(low, high));
    }
    
    int max_p_count = 0;
    int min_longest_dist = INT_MAX;
    vector<int> bar_loc;
    for (size_t i = 0; i < border_indices.size(); i += 1)
    {
        int high = border_indices[i].second;
        int low = border_indices[i].first;
        
        int p_count = hight - low + 1;
        if(max_p_count <= p_count) {
            max_p_count = p_count;
            
            //compute longest dist for this window
            int longest_dist = high - low;
            if(longest_dist % 2 == 0) {//even
                longest_dist = longest_dist / 2;
            }
            else {//odd
                longest_dist = longest_dist / 2 + 1;
            }
            
            //check if minimum longest distance is better
            if(longest_dist < )
            
        }
        
        
    }
    
    
    
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


