#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <cmath>
#include "prettyprint.hpp"
#include <unordered_map>

// Namespaces
using namespace std;

typedef vector< pair<int, int> > vecpair;
typedef vector< vector<int> > Vmap;

//global variables
int n_players, m_att, k_str;
vector<int> def;
vecpair intervals;
Vmap memo;

int itv_length(const pair<int, int>& itv) {
    return itv.second - itv.first + 1;
}

int max_length(int pos, int n_taken) {

    if(memo[pos][n_taken] != -1)
        return memo[pos][n_taken];

    if(n_taken == m_att)
        return 0;

    if(pos >= intervals.size())
        return INT_MIN;

    //general case
    //take
    int start = intervals[pos].second + 1;
    auto next = lower_bound(intervals.begin(), intervals.end(), make_pair(start, start));
    int opt1 = itv_length(intervals[pos]) + max_length(distance(intervals.begin(), next), n_taken+1);
    //leave
    int opt2 = max_length(pos+1, n_taken);
    
    memo[pos][n_taken] = max(opt1, opt2);
    return memo[pos][n_taken];
}

int edf() {
    int counter = 1;
    auto prev = intervals[0];
    for (int i = 1; i < intervals.size(); i += 1)
    {
        auto next = intervals[i];
        if(prev.second < next.first) {
            counter++;
            prev = next;
        }
    }
    return counter;
}


// Functions
// ========= 
void testcases() {
    cin >> n_players >> m_att >> k_str;
    
    def = vector<int>(n_players);
    for (int i = 0; i < n_players; i += 1)
    {
        cin >> def[i];
    }
    
    memo = Vmap(n_players + 2, vector<int>(m_att+2, -1));
    
    //scanline to get relevant intervals
    intervals.clear();
    intervals.reserve(n_players);
    
    int left = 0, right = 0;
    int defsum = def[0];
    while(left < n_players) {
    
        if(defsum == k_str)
            intervals.push_back({left, right});

        if(defsum < k_str && right < n_players -1) {
            right++;
            defsum += def[right];
        }
        else {
            defsum -= def[left];
            left++;
        }
    }
    
    if(intervals.size() < m_att) {
        cout << "fail\n";
        return;
    }

    int len = max_length(0, 0);
        
    if(len < m_att)
        cout << "fail" << endl;
    else
        cout << len << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


