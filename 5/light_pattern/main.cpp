#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>
#include "prettyprint.hpp"

using namespace std;

std::vector<int> pattern;
std::vector<int> lights;
int k;

vector< vector<int> > memo;

int min_switches(const int i, const int invert) {
	
	// check if we go outside of the memo
	if(i==-1) {
		return 0;
	}

	// check if the memo knows the answer
	if(memo[i][invert] != -1)
		return memo[i][invert];

	assert(i >= 0);
	assert(i < lights.size());

	int res;
	// invert lights[i] if 'invert==1' with XOR operator '^'
	if((lights[i] ^ invert) != pattern[i]) {
		//single flip
		res = min_switches(i-1, invert) + 1;

		//if possible flip whole sequence
		if(i % k == k-1) { //is this last light of a sequence?
			int opt = min_switches(i-1, 1- invert) + 1; //flip sequence
			res = min(res, opt);
		}
	}
	else {
		//single flip makes no sense, continue one lower
		res = min_switches(i-1, invert);

		//or does it make sense to flip the whole sequence
		if(i % k == k-1) { //is this last light of a sequence?
			int opt = min_switches(i, 1 - invert) + 1; //flip sequence
			res = min(res, opt);
		}
	}

	memo[i][invert] = res;
	return memo[i][invert];
}

void testcase() {
	int n_lights, exp_pattern;
	cin >> n_lights >> k >> exp_pattern;

	memo = vector< vector<int> >(n_lights, vector<int>(2, -1));

	pattern = std::vector<int>(n_lights);
	for (int j = 0; j < n_lights/k; ++j)
	{
		for (int i = 0; i < k; ++i)
		{
			//watch out when converting the pattern
			//MSB should come first in the vector
			pattern[j*k + i] = (exp_pattern >> (k-i-1)) & 1;
			assert(pattern[j*k + i] == 1 || pattern[j*k + i] == 0);
		}	
	}

	assert(n_lights % k == 0);
	lights = vector<int>(n_lights);
	for(int i=0; i<n_lights; i++) {
		cin >> lights[i];
	}

	cout << min_switches(n_lights-1, 0) << endl;
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