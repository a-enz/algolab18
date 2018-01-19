#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <climits>
#include <algorithm>
#include <unordered_map>
#include <boost/functional/hash.hpp>

using namespace std;

unordered_map< vector<int>, int, boost::hash< vector<int> > > memo;


int min_path(const vector< vector<int> >& adj_list, int v, int mini) {

    //check memo
    vector<int> fvalues = {v, mini};
    auto res = memo.find(fvalues);
    if(res != memo.end()) {
        return res->second;
    }

    if(v == adj_list.size()) {
        memo[fvalues] = 0;
        return memo[fvalues];
    }

	if(mini == 1) {
		int best = INT_MAX;
		for(int nb : adj_list[v]) {
			best = min(best, min_path(adj_list, nb, 0));
		}
        memo[fvalues] = best+1;
        return memo[fvalues];
	} 
	else {
        int best = 0;
        for(int nb : adj_list[v]) {
            best = max(best, min_path(adj_list, nb, 1));
        }
        memo[fvalues] = best+1;
        return memo[fvalues];
	}
}

void testcase() {

	int n, m, r_start, b_start;
	cin >> n >> m >> r_start >> b_start;

	vector< vector<int> > adj_list(n);

	//read transition, add the inverse edge
	for(int i=0; i<m; i++) {
		int from, to;
		cin >> from >> to;

		adj_list[from].push_back(to);
	}

    memo.clear();

    int sherlock = min_path(adj_list, r_start, 1);
    int moriarty = min_path(adj_list, b_start, 1);

    if(sherlock < moriarty)
        cout << 0 << endl;
    else if(moriarty < sherlock)
        cout << 1 << endl;
    else if(sherlock % 2 == 1)
        cout << 0 << endl;
    else 
        cout << 1 << endl;

}

int main(void) {

	//disable for performance
	ios::sync_with_stdio(false);
	//read total number of test cases
	int n_total_testcases;
	cin >> n_total_testcases;

	//do for each testcase
	for(int n_test = 0; n_test < n_total_testcases; n_test++) {
		testcase();
	}

	return 0;
}