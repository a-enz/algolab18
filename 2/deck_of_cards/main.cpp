#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits.h>
#include <iterator>
#include <assert.h>
#include <signal.h>

using namespace std;

void myassert(bool is_valid) {
	if(not is_valid) raise(SIGSEGV);
}

void testcase() {

	//read the input
	int n_cards, k;
	cin >> n_cards >> k;
	vector<int> values(n_cards);

	for(int c=0; c<n_cards; c++) {
		cin >> values[c];
	}

	//do scanline over 'values'
	//initialize:
	vector<int>::iterator left=values.begin(), right=values.begin();
	int i=0, j=0, k_best_approx=*left, sum=*left;

	while(left != values.end()) {
		myassert(left <= right);
		myassert(sum >= 0);


		//check if the new approximation is better than the
		//previous one and update
		if(sum==k){
			i = distance(values.begin(), left);
			j = distance(values.begin(), right);
			break;
		}
		else if(abs(k - sum) < abs(k - k_best_approx)) {
			k_best_approx = sum;
			i = distance(values.begin(), left);
			j = distance(values.begin(), right);
		}

		//update approximation by increasing left or
		//right pointer depending on if we the sum is larger
		//or smaller than the target value k
		if(sum < k/* && right != values.end()*/) {
			if(right == values.end()) break;
			right++;
			sum += *right;
		}
		else {
			if(left == right) {
				right++;
				sum += *right;
			}

			sum -= *left;
			left++;
		}

	}

	//write output values for testcase
	cout << i << " " << j << endl;
}

int main(void) {

	//disable for performance
	ios::sync_with_stdio(false);
	//read total number of test cases
	int n_total_testcases;
	cin >> n_total_testcases;

	for(int n_test = 0; n_test < n_total_testcases; n_test++) {
			testcase();
	}

	return 0;
}