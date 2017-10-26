#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <assert.h>
#include <map>
#include <queue>

using namespace std;

typedef map<int, queue<int> >::iterator		timer_it;

void testcase() {
	int n_bombs;
	cin >> n_bombs;

	map<int, queue<int> > timers;
	vector<bool> visited(n_bombs, false);

	for(int i=0; i<n_bombs; i++) {
		int timer;
		cin >> timer;

		try{
			timers.at(timer).push(i);
		}
		catch(const out_of_range& oor) {
			queue<int> q;
			q.push(i);
			timers.insert(pair<int, queue<int> >(timer, q));
		}
	}

	bool exploded = false;
	timer_it t_it = timers.begin();
	int time = 1;

	while(t_it->first <= n_bombs) {
		int current_time = t_it->first;
		queue<int> q = t_it->second;

		//check if the time is still valid
		if(current_time < time) {
			exploded = true;
			break;
		}

		while(not q.empty()) {

			int node = q.pop();

			if(not visited[node]) {
				//check if children visited if bomb not in bottom level
				int start_lowest_lvl = n_bombs / 2;
				if(node > start_lowest_lvl) {

					//if children visited and bomb timer not too early
					//increment timer and set bomb to visited

					//if children not visited check tree downwards
				} else { //node in last level
					time++;
				}
			}
		}

		t_it++;
	}

	if(exploded)
		cout << "no\n";
	else
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