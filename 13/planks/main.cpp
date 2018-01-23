#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <climits>
#include <cassert>
#include "prettyprint.hpp"

using namespace std;

typedef vector< vector<int> > vecvec;

//global variable
std::vector<long> length;
long side_length;

void subsetsums(int i, int stop, vector<int>& sum_set, 
					vecvec& list) {
	//base case
	if(i == stop-1) {
		//store sum at this point in time
		list.push_back(sum_set);
		return;
	}

	//otherwise push assignments
	for (int j = 0; j < 4; ++j)
	{
		sum_set[j] += length[i];
		subsetsums(i-1, stop, sum_set, list);
		sum_set[j] -= length[i];
	}
}

void testcase() {
	int n_planks;
	cin >> n_planks;

	length = vector<long>(n_planks);
	long total_length = 0;
	long max_length = 0;
	for (int i = 0; i < n_planks; ++i)
	{
		cin >> length[i];
		total_length += length[i];
		max_length = max(max_length, length[i]);
	}

	//do some easy checks
	//total length not divisible by 4
	//or a plank longer than a side
	if(total_length % 4 != 0 || max_length > total_length / 4) {
		cout << "0\n";
		return;
	}

	side_length = total_length / 4;

	//compute List of subsetsums for 4-tuples
	vector<int> sum_set1(4, 0), sum_set2(4,0);
	vecvec List1, List2;
	subsetsums(n_planks-1, n_planks / 2, sum_set1, List1);
	subsetsums(n_planks / 2 - 1, 0, sum_set2, List2);

	//sort F2
	sort(List2.begin(), List2.end());

	//iterate over all elements in L1 and check how many of 
	//the "inverse element" in F2 exist
	long c = 0;
	for (vector<int> sums : List1)
	{
		vector<int> inverse(4, side_length);
		for (int i = 0; i < 4; ++i)
		{
			inverse[i] -= sums[i];
		}

		//first element that is equal to 'inverse'
		auto lb = lower_bound(List2.begin(), List2.end(), inverse);
		//first element larger than 'inverse'
		auto ub = upper_bound(List2.begin(), List2.end(), inverse);

		c += distance(lb, ub);
	}


	cout << c / 24  << endl;
	


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