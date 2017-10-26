#include <iostream>
#include <vector>
#include <cmath>

using namespace std;



int n_choose_2(int n) {
	return n * (n-1) / 2;
}

int main(void) {

	//disable for performance
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);
	//read total number of test cases
	int n_total_testcases;
	cin >> n_total_testcases;

	//do for each testcase
	for(int n_test = 0; n_test < n_total_testcases; n_test++) {

		int n_values, val = 0, c_even = 0, c_odd = 0, partial_sum = 0;
		cin >> n_values;


		for(int i = 0; i < n_values; i++) {
			cin >> val;
			partial_sum += val;

			if(partial_sum % 2 == 0) 
				c_even += 1;
			else
				c_odd += 1;
		}

		int total_even = n_choose_2(c_even) + n_choose_2(c_odd) + c_even;
		//write output values for testcase
		cout << total_even << '\n';
	}

	return 0;
}
