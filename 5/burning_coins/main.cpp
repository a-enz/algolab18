#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>

using namespace std;

//use a fixed table as memo since we know it won't be too big
//and access time is constant instead of logarithmic when using a map
vector< vector<int> > memo(2500, vector<int>(2500,0));

int maximize_money(int start, int end, const vector<int>& value_coin) {
	//pair<int, int> p = make_pair(start, end);
	if(memo[start][end] == 0) { //no value stored yet

		int length_remaining = end-start+1;
		if(length_remaining<=2) { //for ==1 and ==2
			memo[start][end] = max(value_coin[start], value_coin[end]);
		}
		else {
			//we take the start coin
			int variant_start_val = value_coin[start];

			variant_start_val += 
				min(maximize_money(start+1, end-1, value_coin),
					maximize_money(start+2, end, value_coin));



			//take the end coin
			int variant_end_val = value_coin[end];

			variant_end_val += 
				min(maximize_money(start, end-2, value_coin), 
					maximize_money(start+1, end-1, value_coin));

			memo[start][end] =  max(variant_start_val, variant_end_val);
		}
	}

	return memo[start][end];
}


void testcase() {
	int n_coins;
	cin >> n_coins;

	memo = vector< vector<int> >(2500, vector<int>(2500,0));

	vector<int> value_coin(n_coins);

	for(int i=0; i<n_coins; i++) {
		cin >> value_coin[i];
	}

	//compute max

	int max = maximize_money(0, n_coins-1, value_coin);

	cout << max << endl;
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