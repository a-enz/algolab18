#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <algorithm>

using namespace std;

int n;
vector< vector<int> > coin;
vector< vector< vector<int> > > memo;

int best_paths(int d, int x1, int x2) {
    int y1 = d - x1;
    int y2 = d - x2;

    //if we went outside of the level, return a very low value to punish
    if(x1 == n || x2 == n || y1 == n || y2 == n) return INT_MIN;

    //if precomputed result exists, return
    if(memo[d][x1][x2] != -1) return memo[d][x1][x2];

    //otherwise compute result recursively
    int result;
    if(x1 == x2) {
        if(x1 == n-1) return coin[n-1][n-1];
        else {
            result =  coin[x1][y1] + 
                max(best_paths(d+1, x1+1, x2), best_paths(d+1, x1, x2+1));
        }
    } 
    else {
        result =  coin[x1][y1] + coin[x2][y2] + max({
                best_paths(d+1, x1+1, x2),
                best_paths(d+1, x1, x2),
                best_paths(d+1, x1, x2+1),
                best_paths(d+1, x1+1, x2+1)
            });

    }

    memo[d][x1][x2] = result;
    return result;
}

void testcase() {
	cin >> n;

    memo = vector< vector< vector<int> > >(2*n, vector< vector<int> >(n, vector<int>(n, -1)));

    coin = vector< vector<int> > (n, vector<int>(n));
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            cin >> coin[i][j];
        }
    }

    cout << best_paths(0, 0, 0) << endl;
    	
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