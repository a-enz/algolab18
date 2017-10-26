#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <algorithm>

using namespace std;


enum weight_info {
	NONE,
	SMALLER,
	LARGER,
	EQUAL
};

void update(weight_info *pos, weight_info val) {
	if(val == LARGER && *pos == SMALLER) 
		*pos = EQUAL;
	else if(val == SMALLER && *pos == LARGER) 
		*pos = EQUAL;
	else if(*pos == NONE) 
		*pos = val;
	//should only encounter above cases
}

int main(void) {

	//disable for performance
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);
	//read total number of test cases
	int n_total_testcases;
	cin >> n_total_testcases;

	//do for each testcase
	for(int n_test = 0; n_test < n_total_testcases; n_test++) {

		int n_weightings, total_coins;
		cin >> total_coins >> n_weightings;

		vector<weight_info> suspicous_coins(total_coins);

		for(int i = 0; i < n_weightings; i++) {
			int n_weighted_coins;
			cin >> n_weighted_coins;

			vector<int> weighted_coins_left(n_weighted_coins);
			vector<int> weighted_coins_right(n_weighted_coins);

			//left side
			for(int j = 0; j < n_weighted_coins; j++) {
				int val;
				cin >> val;
				weighted_coins_left[j] = val;
			}

			//right side
			for(int j = 0; j < n_weighted_coins; j++) {
				int val;
				cin >> val;
				weighted_coins_right[j] = val;
			}

			char balance;
			cin >> balance;


			if(balance == '=')
			{
				for(int val : weighted_coins_left) {
					suspicous_coins[val-1] = EQUAL;
				}

				for(int val : weighted_coins_right) {					
					suspicous_coins[val-1] = EQUAL;
				}
			}
			else if(balance == '<') 
			{	
				vector<bool> seen_coins(total_coins, false);

				for(int val : weighted_coins_left) {
					update(&(suspicous_coins[val-1]), SMALLER);
					seen_coins[val-1] = true;
				}

				for(int val : weighted_coins_right) {
					update(&(suspicous_coins[val-1]), LARGER);
					seen_coins[val-1] = true;
				}

				for(int c = 0; c < total_coins; c++) {
					if(not seen_coins[c]) 
						suspicous_coins[c] = EQUAL;
				}
			}
			else //'>'
			{
				vector<bool> seen_coins(total_coins, false);

				for(int val : weighted_coins_left) {
					update(&(suspicous_coins[val-1]), LARGER);
					seen_coins[val-1] = true;
				}

				for(int val : weighted_coins_right) {
					update(&(suspicous_coins[val-1]), SMALLER);
					seen_coins[val-1] = true;
				}

				for(int c = 0; c < total_coins; c++) {
					if(not seen_coins[c]) 
						suspicous_coins[c] = EQUAL;
				}
			}

			/*
			cout << "still suspicous:\n";
			for(int c = 0; c < total_coins; c++) {
				if(suspicous_coins[c] != EQUAL) cout << c+1 << " ";
			}
			cout << endl;
			*/
		}

		int count=0, bad_coin;
		for(int i = 0; i < total_coins; i++){
			if(suspicous_coins[i] != EQUAL) {
				count++;
				bad_coin = i+1;
			}
		}

		if(count != 1)
			cout << 0 << endl;
		else 
			cout << bad_coin << endl;
	}

	return 0;
}