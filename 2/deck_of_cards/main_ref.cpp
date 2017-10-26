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
	int i=0, j=0, i_sol=0, j_sol=0, sum=values[0], best=sum;

	while(i < n_cards) {

		myassert(i<=j);
		myassert(sum>=0);
		
		if(abs(best-k) > abs(sum-k)) {
			j_sol = j;
			i_sol = i;
			best = sum;
		}

		if (sum < k && j < n_cards) {
			j++;
			sum += values[j];
		}
		else
		{	
			if(i==j) {
				j++;
				sum += values[j];
			}

			sum -= values[i];
			i++;
		}
	}


	//write output values for testcase
	cout << i_sol << " " << j_sol << endl;
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