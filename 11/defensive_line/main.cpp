
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <queue>
#include <map>
#include <unordered_map>
#include <cassert>
#include <boost/functional/hash.hpp>


// Namespaces
using namespace std;

//global variables
vector< pair<int, int> > intervals; //[a,b] as defined in the exercise
vector<int> intervals_start;
//unordered_map< pair<int, int>, int, boost::hash< pair<int, int> > > memo;
map< pair<int, int>, int> memo;
    

    
int maximize_sum(const int idx, int k) { //k: number of elements we still need to take
    auto current = intervals[idx];
    int size = current.second - current.first + 1; //size current interval
    
    //check if the memo knows the solution    
    int res;
    pair<int, int> inputs = make_pair(idx, k);
    auto sol = memo.find(inputs);
    if(sol != memo.end())
        return sol->second;
    

    if(k == 0){ //can't take any more intervals{
        res = 0;
        memo.insert(make_pair(inputs, res));
        return res;
    }
    
    //base case
    if(idx == intervals.size()-1) {
        res = size;
        memo.insert(make_pair(inputs, res));
        return res;
    }
        
    //normal case
    //find next possible interval if we want to use current
    
    auto next_it = upper_bound(intervals_start.begin(), intervals_start.end(), current.second);
    if(next_it == intervals_start.end()) {
        res =  max(size, maximize_sum(idx+1, k));
        memo.insert(make_pair(inputs, res));
        return res;
    }
    
    
    int next_idx = distance(intervals_start.begin(), next_it);
    
    res =  max(size + maximize_sum(next_idx, k-1), maximize_sum(idx+1, k));
    memo.insert(make_pair(inputs, res));
    return res;
}



void testcases() {
    int n, m; long k;
    cin >> n >> m >> k;
    
    vector<int> defense(n);
    for (unsigned int i = 0; i < n; i += 1)
    {
        cin >> defense[i];
    }
    
    
    //Make sure the memo is empty
    memo.clear();
    
    
    //copute all existing intervals where the sum 
    //of defense values is k.
    //done by constructing a map of the sum values
    //s.t. we can look up the indices of m[s_i - k]
    intervals.clear(); //[a,b] as defined in the exercise
    intervals_start.clear();
    vector<long> sums(n);
    unordered_map<long, int> sum_idx; //map sum to idx
    sums[0] = defense[0];
    sum_idx.insert(make_pair(0, -1));
    sum_idx.insert(make_pair(defense[0], 0));
    for (int i = 1; i < n; i += 1)
    {
        sums[i] = sums[i-1] + defense[i];
        sum_idx.insert(std::make_pair(sums[i], i));
    }
    
    for (int i = 0; i < n; i += 1)
    {
        auto idx = sum_idx.find(sums[i] - k);
        
        if(idx != sum_idx.end()) {
        
            int other = (idx->second == -1) ? 0 : sums[idx->second];
            assert(sums[i] - other == k);
            intervals.push_back(make_pair((idx->second)+1, i));
            intervals_start.push_back((idx->second)+1);
        }
    }

    //end early if we don't even have enough intervals
    if(intervals.size() < m) {
        cout << "fail\n";
        return;
    }
    
    //first check if there is at least one solution with n 
    //non overlapping intervals by checking greedily
    pair<int, int> current = intervals[0];
    int counter = 1;
    for (unsigned int i = 1; i < intervals.size(); i += 1)
    {
        //move until left border of next interval is after right border of current
        pair<int, int> next = intervals[i];
        bool stop = false;
        while(current.second >= next.first) {
            i++;
            if(i < intervals.size()) {
                next = intervals[i];
            }
            else {
                stop = true;
                break;
            }
        }
        
        if(not stop) {
            counter++;
            current = next;
        }
        else 
            break;
    }
    
    if(counter < m) {
        cout << "fail\n";
        return;
    } 
    else 
        cout << maximize_sum(0, m) << endl;

}

// Main function looping over the testcases
int main() {
	//ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


