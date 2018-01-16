#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <set>

using namespace std;


void testcase() {
	int n_friends, m_boxes;
	cin >> n_friends >> m_boxes;

	vector<int> strength(n_friends);
	for (int i = 0; i < n_friends; ++i)
	{
		cin >> strength[i];
	}

	multiset<int, std::greater<int> > weights;
	for (int i = 0; i < m_boxes; ++i)
	{	
		int w;
		cin >> w;
		weights.insert(w);
	}

	sort(strength.begin(), strength.end(), std::greater<int>());
	if(*weights.begin() > strength[0]) {
		//found a weight heavier than maximal strength
		cout << "impossible\n";
		return;
	}

	int rounds = 0;
	while(weights.size() != 0) {
		int i=0;
		auto box = weights.lower_bound(strength[i]);
		while(i < n_friends && box != weights.end()) {
			weights.erase(box);
			i++;
			box = weights.lower_bound(strength[i]);
		}
		rounds++;
	}

	int time = rounds * 3 - 1;
	cout << time << endl;
	//write output values for testcase
}

int main(void) {

	//disable for performance
	ios::sync_with_stdio(false);
	//read total number of test cases
	int n_total_testcases;
	cin >> n_total_testcases;

	//do for each testcase
	for(int n_test = 0; n_test < n_total_testcases; n_test++) {
		testcase();
	}

	return 0;
}