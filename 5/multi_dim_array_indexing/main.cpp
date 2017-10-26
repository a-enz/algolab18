#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;



void testcase() {
	int k, n, q;
	cin >> k >> n >> q;

	//read bounds
	for(int i=0; i<k; i++) {
		int lower, upper;
		cin >> lower >> upper;
	}

	//read param values
	map<string, int> param_values;
	for(int i=0; i<n; i++) {
		string params="";
		for(int j=0; j<k; j++) {
			string p;
			cin >> p;
			params += p;
		}
		int res;
		cin >> res;
		param_values[params] = res;
	}

	//read queries
	vector<string> queries(q);
	for(int i=0; i<q; i++) {
		string params="";
		for(int j=0; j<k; j++) {
			string p;
			cin >> p;
			params += p;
		}
		queries[i] = params;

		if(param_values.find(queries[i]) != param_values.end()) {
			cout << param_values[queries[i]] << endl;
		} 
		else {
			cout << "-1\n";
		}
	}
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