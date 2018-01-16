#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <queue>

using namespace std;

void testcase() {
	int n_bombs;
	cin >> n_bombs;

	vector<int> timer(n_bombs);

	for (int i = 0; i < n_bombs; ++i)
	{
		cin >> timer[i];
	}

	// traverse the tree and keep track of the explosion timers
	vector<int> order;
	order.reserve(n_bombs);

	//queue of pairs (timer[i], i)
	//ensure queue does the right comparison
	auto comp = [&](const int& i, const int& j) -> bool{
		return timer[i] < timer[j];
	};

	priority_queue<int, vector<int>, decltype(comp)> Q(comp);

	Q.push(0);

	while(not Q.empty()) {
		int j = Q.top();
		Q.pop();
		order.push_back(timer[j]);

		if(j <= (n_bombs - 3) / 2) {
			Q.push(2*j + 1);
			Q.push(2*j + 2);	
		}
	}

	int defuse_counter = 1;
	for (int i = order.size()-1; i >= 0; --i)
	{
		if(order[i] < defuse_counter) {
			cout << "no\n";
			return;
		}
		defuse_counter++;
	}

	cout << "yes\n";
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