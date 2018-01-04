#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

void testcase() {
	int n;
	cin >> n;

    vector< vector<int> > coins(n, vector<int>(n));
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            cin >> coin[i][j];
        }
    }

	//write output values for testcase
	
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