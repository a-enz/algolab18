#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <assert.h>
#include <map>

using namespace std;

void testcase() {
	int n_bombs;
	cin >> n_bombs;

	vector<int> bomb_timers(n_bombs);
	vector<bool> visited(n_bombs);

	for(int i=0; i<n_bombs; i++) {
		cin >> bomb_timers[i];
	}

	int timer = n_bombs;
	bool exploded=false;


	//do DFS on the heap of bombs, checking the timers 'backwards'
	for(int level=0; pow(2,level)-1<n_bombs; level++) {
		//cout << "level " << level << endl;
		int left=pow(2,level)-1, right=pow(2,level+1)-1;
		int level_timer = timer - pow(2,level+1) + 1;
		//cout << "level timer " << level_timer << endl;
		
		//cout << "searching interval [" << left << ", " << right << "]" << endl;

		assert(right<=n_bombs);

		for(int i=left; i<right; i++) {
			if(bomb_timers[i] <= level_timer) {
				exploded=true;
				break;
			}

			//and check if there are several with the same time
		}
		if(exploded) break;
	}


	//write output values for testcase
	if(not exploded) 
		cout << "yes\n";
	else
		cout << "no\n";
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