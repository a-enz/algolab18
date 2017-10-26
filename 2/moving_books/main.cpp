#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

struct worker_category {
	int max_weight;
	int total_workers;
	int remaining_boxes;
}

bool wc_compare(const worker_category& a, const worker_category& b) {
	return a.remaining_boxes < b.remaining_boxes;
}

void testcase() {
	int n_friends, m_boxes;
	cin >> n_friends >> m_boxes;

	vector<int> strengths(n_friends), weights(m_boxes);
	for(int i=0; i<n_friends; i++) {
		cin >> strengths[i];
	}

	for(int i=0; i<m_boxes; i++) {
		cin >> weights[i];
	}

	sort(strengths.begin(), strengths.end(), std::greater<int>());
	sort(weights.begin(), weights.end(), std::greater<int>());

	//for each strenght interval k keep track of amount of boxes and 
	//number of friends that can be classified into that interval
	vector<worker_category> worker_pools;

	
	int prev_s = strengths[0];
	int category = 0;
	worker_pools.push_back(1);

	int w = 0;
	for(int i=1; i<n_friends; i++) {
		if(prev_s != strengths[i]){ //because we sorted
			worker_pools.push_back(1);
			prev_s = strengths[i];
			category++;

			//add all boxes between new and previous category
			int count=0;
			while(weights[w] > prev_s) {//works because we sorted
				w++;
				count++;
			}
			box_piles.push_back(count);
		}
		else {
			worker_pools[category]++;
		}
	}

 	//add last few boxes we did not process in the loop
	box_piles.push_back(weights.size() - w);



	//now do the processing
	int total_time=0;

	while(true) {

		//find shortest interval

	}

	cout << "test";

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