#include <iostream>
#include <vector>
#include <assert.h>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <boost/functional/hash.hpp>

using namespace std;

struct v_hash {
	size_t operator()(const vector<int> &v) const {
		return boost::hash_value(v);
	}
};


unordered_map< vector<int>, int, v_hash> memo;


//compute the subsets we want to check
//(subsets of the same color)
unordered_map<int, int> find_color_subsets(
				const vector< vector<int> >& stack,
				vector<int> top_coin) {

	unordered_map<int, int> result;
	for(int i=0; i<stack.size(); i++) {
		if(top_coin[i] < 0) {
			//there is no coin to look at here
			continue;
		}

		int color = stack[i][top_coin[i]];

		auto item = result.find(color);
		if(item != result.end()) {
			//update integer representing color subset
			int subset = item->second;
			result[color] = subset + (1<<i);
		}
		else {//add the item
			result[color] = (1<<i);
		}
	}
	return result;
}

int maximize_score(const vector< vector<int> >& stack,
					vector<int> top_coin,
					int bitset) {
	//precondition:
	//the subset given in 'bitset' on the coins 'top_coin' in the 'stack'
	//consists of coins of the same color

	vector<int> func_value = top_coin;
	func_value.push_back(bitset);

	//check if we have already computed this function before
	auto item = memo.find(func_value);
	if(item != memo.end()) {
		return item->second;
	} //else continue to compute


	//compute the score of the color subset given in 'bitset'
	//count the '1' bits in 'bitset'
	//also reduce the top_coin values for the coins we take
	int subset_size = 0;
	for(int i=0; i<stack.size(); i++) {
		if(((bitset>>i) & 1) == 1) {
			subset_size++;
			top_coin[i]--;
		}
	}

	int score = (subset_size == 1) ? 0 : pow(2, subset_size-2);


	unordered_map<int, int> color_subset = 
				find_color_subsets(stack, top_coin);


	//with updated top coins start new subroutine
	int max_score = 0;

	for(unordered_map<int, int>::iterator it = color_subset.begin();
		it != color_subset.end();
		it++) {
		//with i we model which subset of the top coins we take
		//e.g.:
		//00001 would be only coin of stack 1 taken
		//11111 would be top coins of all stacks
		int new_max = maximize_score(stack, top_coin, it->second);
		max_score = max(max_score, new_max);
	}

	memo.insert(make_pair(func_value, score+max_score));
	return score + max_score;
}


void testcase() {
	int n_stacks;
	cin >> n_stacks;

	//stacks are constructed such that:
	//chips at the bottem have index 0:
	//e.g: stack[4][0] is the color at the very bottom of stack 4
	vector< vector<int> > stack(n_stacks, vector<int>());
	vector<int> stacksize(n_stacks);


	vector<int> top_coin(n_stacks);

	//read stack size
	for(int i=0; i<n_stacks; i++) {
		cin >> stacksize[i];
		top_coin[i] = stacksize[i]-1;
	}

	//set up memo 
	memo = unordered_map< vector<int>, int, v_hash>();

	//read values of chips
	for(int s=0; s<n_stacks; s++) {
		for(int i=0; i<stacksize[s]; i++) {
			int val;
			cin >> val;
			stack[s].push_back(val);
		}
	}

	unordered_map<int, int> color_subset = 
				find_color_subsets(stack, top_coin);


	int max_score = 0;

	for(unordered_map<int, int>::iterator it = color_subset.begin();
		it != color_subset.end();
		it++) {
		//with i we model which subset of the top coins we take
		//e.g.:
		//00001 would be only coin of stack 1 taken
		//11111 would be top coins of all stacks
		int new_max = maximize_score(stack, top_coin, it->second);
		max_score = max(max_score, new_max);
	}

	cout << max_score << endl;

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