#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <algorithm>

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

		int n_weightings, total_coins;
		cin >> total_coins >> n_weightings;

		vector<int> suspicous_coins(total_coins);
		vector< vector<bool> > left_right_occ(total_coins, vector<bool>(2, false));

		for(int i = 0; i < total_coins; i++) {
			suspicous_coins[i] = i;
		}

		for(int i = 0; i < n_weightings; i++) {
			int n_weighted_coins;
			cin >> n_weighted_coins;

			vector<int> weighted_coins;

			for(int j = 0; j < 2 * n_weighted_coins; j++) {
				int val;
				cin >> val;
				left_right_occ[val] = 
				weighted_coins.push_back(val);
			}


			sort(weighted_coins.begin(), weighted_coins.end());			

			char balance;
			cin >> balance;

			vector<int> result(total_coins);
			vector<int>::iterator result_it;

			if(balance == '=')
			{
				result_it = set_difference(suspicous_coins.begin(), suspicous_coins.end(), 
										weighted_coins.begin(), weighted_coins.end(),
										result.begin());
			}
			else if(balance == '<') 
			{
				result_it = set_intersection(suspicous_coins.begin(), suspicous_coins.end(), 
										weighted_coins.begin(), weighted_coins.end(),
										result.begin());
			}
			else //'>'
			{
				result_it = set_intersection(suspicous_coins.begin(), suspicous_coins.end(), 
										weighted_coins.begin(), weighted_coins.end(),
										result.begin());
			}

			result.resize(result_it - result.begin());
			suspicous_coins = result;

			cout << "set now contains\n";
			for(auto it = suspicous_coins.begin(); it != suspicous_coins.end(); it++) {
				cout << *it << " ";
			}
			cout << endl;
		}

		if(suspicous_coins.size() != 1)
			cout << 0 << endl;
		else 
			cout << suspicous_coins.front() << endl;
	}

	return 0;
}