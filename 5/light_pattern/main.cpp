#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

void testcase() {
	int n_lights, k, exp_pattern;
	cin >> n_lights >> k >> exp_pattern;

	vector<int> light_pattern(n_lights/k);
	cout << "exp_pattern " << exp_pattern << endl;
	for(int i=0; i<n_lights/k; i++) {
		int pattern=0;
		for(int j=k-1; j>=0; j--) {
			int state;
			cin >> state;

			pattern += state << j;
		}

		cout << pattern << " ";
	}

	cout << endl;




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