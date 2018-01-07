// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <assert.h>
#include <set>

// Namespaces
using namespace std;

void add_bars(vector<int> &bar_loc, vector<int> &b) {
    
    for (unsigned int i = 0; i < b.size(); i += 1)
    {
        bar_loc.push_back(b[i]);
    }
}

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
        //cout << "high " << loc[high]  << " low " << loc[low] << endl;
        int p_count = high - low + 1;
        
        //compute longest dist for this window
        //and bar locations
        int longest_dist = loc[high] - loc[low];
        assert(longest_dist <= 200);
        vector<int> b;
        if(longest_dist % 2 == 0) {//even
            longest_dist = longest_dist / 2;
            b.push_back(loc[low] + longest_dist);
        }
        else {//odd
            longest_dist = longest_dist / 2 + 1;
            b.push_back(loc[low] + longest_dist);
            b.push_back(loc[high] - longest_dist);
        }
        
        //checks on optimality condition
        if(max_p_count == p_count) {
            if(min_longest_dist > longest_dist) { 
                bar_loc.clear();
                min_longest_dist = longest_dist;
                add_bars(bar_loc, b);
            }
            else if(min_longest_dist == longest_dist) {
                add_bars(bar_loc, b);
            }
        }
        else if(max_p_count < p_count) {
            bar_loc.clear();
            min_longest_dist = longest_dist;
            max_p_count = p_count;
            add_bars(bar_loc, b);
        }
    }
    
    sort(bar_loc.begin(), bar_loc.end());
    
    cout << max_p_count << " " << min_longest_dist << endl;
    for (unsigned int i = 0; i < bar_loc.size(); i += 1)
    {
        cout << bar_loc[i] << " ";
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


