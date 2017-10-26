#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits.h>

using namespace std;

int main(void) {

	//disable for performance
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);
	//read total number of test cases
	int n_total_testcases;
	cin >> n_total_testcases;

	//do for each testcase
	for(int n_test = 0; n_test < n_total_testcases; n_test++) {

		int n_dominoes;
		cin >> n_dominoes;

		vector<int> domino_lengths(n_dominoes);
		for(int i=0; i<n_dominoes; i++) {
			cin >> domino_lengths[i];
		}

		int pos_current=0;

		while(pos_current < n_dominoes) {
			//find max
			int max = 0, pos_new = pos_current;
			int right_bound = pos_current + domino_lengths[pos_current];
			right_bound = (right_bound > n_dominoes) ? n_dominoes : right_bound;

			//cout << "searching interval [" << pos_current << ", " << right_bound << ") " <<  endl;
			for(int pos = pos_current; pos < right_bound; pos++) {
				int length_new = domino_lengths[pos] - (right_bound - pos);

				if (length_new > max){
					max = length_new;
					pos_new = pos;
				}
			}

			if(pos_current != pos_new) { //another domino falls
				pos_current = pos_new;
			}
			else {
				break; //no domino falls, stop searching
			}

			//cout << "current position " << pos_current << endl;
		}
		
		//need to add the last fallen domino (that did not trigger further dominos)
		//-1: because a domino of length x only reaches (x-1) positions
		//+1: Because the dominos are counted from 1 and not from 0
		pos_current = pos_current + domino_lengths[pos_current]; // -1 + 1;
		pos_current = (pos_current > n_dominoes) ? n_dominoes : pos_current;

		cout << pos_current << '\n';
	}

	return 0;
}