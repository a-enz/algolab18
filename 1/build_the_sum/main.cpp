#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main(void) {

	//disable for performance
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);
	//read total number of test cases
	int n_total_testcases;
	cin >> n_total_testcases;

	//do for each testcase
	for(int n_test = 0; n_test < n_total_testcases; n_test++) {

		int n_summands;
		cin >> n_summands;

		float val, sum = 0;
		for(int i = 0; i < n_summands; i++) {
			cin >> val;
			sum += val;
		}

		//write output values for testcase
		cout << sum << '\n';
	}

	return 0;
}