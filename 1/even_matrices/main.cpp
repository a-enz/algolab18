#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

int n_choose_2(int n) {
	return n * (n-1) / 2;
}

int even_pairs(int n, vector<int> values) {

	int partial_sum = 0, c_even = 0, c_odd=0;
	for(int i = 0; i < n; i++) {
		partial_sum += values[i];

		if(partial_sum % 2 == 0) 
			c_even += 1;
		else
			c_odd += 1;
	}

	int total_even = n_choose_2(c_even) + n_choose_2(c_odd) + c_even;
	return total_even;
}


void run_test() {
	int n;
	cin >> n;

	vector< vector<int> > values(n, vector<int>(n));

	int odd=0, even=0;
	for(int row=0; row < n; row++) {
		for(int col=0; col < n; col++) {
			cin >> values[row][col];
		}
	}

	//compute the partial sum array for each pair i1, i2 s.t.: 1 <= i1 <= i2 <= n
	//Each of these partial sum arrays S_i1_i2 has length n, 
	//Each entry in S_i1_i2[j] is the sum of the column from i1 to i2: 
	//sum(values[i1][j] + ... + values[i2][j])
	//S_i1_i2 := partial_sums[i1][i2]
	vector< vector< vector<int> > > partial_sums(n, vector< vector<int> >(n, vector<int>(n)));
	even=0; odd=0;
	for(int i1=0; i1<n; i1++){
		for(int i2=i1; i2<n; i2++) {
			for(int j=0; j<n; j++) {
				if(i1==i2) {
					partial_sums[i1][i2][j] = values[i1][j];
				}
				else if(i1==0) {
					partial_sums[0][i2][j] = partial_sums[0][i2-1][j] + values[i2][j];
				}
				else {
					partial_sums[i1][i2][j] = partial_sums[i1][i2-1][j]
												+ values[i2][j];
				}

			}
		}
	}


	//Do even_pairs on each S_i1_i2 vector and sum up all the even_count values
	int total_even=0;

	for(int i1=0; i1<n; i1++){
		for(int i2=i1; i2<n; i2++) {
			total_even += even_pairs(n, partial_sums[i1][i2]);
		}
	}

	cout << total_even << endl;
}

int main(void) {

	//disable for performance
	ios::sync_with_stdio(false);
	//read total number of test cases
	int n_total_testcases;
	cin >> n_total_testcases;

	//do for each testcase
	for(int n_test = 0; n_test < n_total_testcases; n_test++) {

		run_test();
	}

	return 0;
}