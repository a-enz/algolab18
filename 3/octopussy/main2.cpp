#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <queue>
#include <tuple>
#include "prettyprint.hpp"

// Namespaces
using namespace std;


// Functions
// ========= 
void testcases() {
    int n_balls;
    cin >> n_balls;
    vector<int> times(n_balls);
    for (unsigned int i = 0; i < n_balls; i += 1)
    {
        cin >> times[i];
    }
    
    priority_queue< tuple<int, int> > Q;
    Q.push({times[0], 0});
    vector<int> taken;
    
    while(not Q.empty()) {
        int time, idx;
        tie(time, idx) = Q.top();
        Q.pop();
        taken.push_back(time);
        
        if(idx <= (n_balls-3) / 2) {
            int c_idx = 2*idx + 1;
            Q.push({times[c_idx], c_idx});
            Q.push({times[c_idx+1], c_idx+1});
        }   
    }
    
//    cout << taken << endl;
    assert(taken.size() == n_balls);
    int tick = 0;
    for (int i = n_balls-1; i >= 0; i--)
    {   
        tick++;
        if(tick > taken[i]) {
            cout << "no\n";
            return;
        }
    }
    
    cout << "yes\n";
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


