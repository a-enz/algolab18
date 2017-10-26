#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits.h>
#include <map>
#include <unordered_set>

using namespace std;

typedef map<int, int>::iterator				pos_it;

void testcase() {
	int n_words;
	cin >> n_words;

	vector<int> word_counts(n_words);
	for(int c=0; c<n_words; c++) {
		cin >> word_counts[c];
	}

	//store a map of pos->word
	map<int, int> pos_map;
	for(int c=0; c<n_words; c++) {
		for(int i=0; i<word_counts[c]; i++) {
			int pos;
			cin >> pos;
			pos_map.insert(pair<int, int>(pos, c));
		}
	}
	

	//initialize values
	pos_it left=pos_map.begin(), right=pos_map.begin();
	int shortest_length=INT_MAX;

	vector<int> included_count(n_words, 0);
	int included_words_count = 1;

	included_count[left->second]++;

	//sliding window over word positions
	while(left != pos_map.end()) {

		if(included_words_count == n_words) {
			int new_length = right->first - left->first + 1;
			shortest_length = min(new_length, shortest_length);
		}

		if(included_words_count < n_words) {
			//include more words
			right++;
			if(right == pos_map.end()) break;

			if(included_count[right->second]==0) included_words_count++;
			included_count[right->second]++;
			
		}
		else {//make the interval shorter, remove words
			if(right == left) {
				//include more words
				right++;

				if(included_count[right->second]==0) included_words_count++;
				included_count[right->second]++;
				
			}

			if(included_count[left->second]==1) included_words_count--;
			included_count[left->second]--;
			
			left++;
		}
	}

	//write output values for testcase
	cout << shortest_length << '\n';
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