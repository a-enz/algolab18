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


int maximize_score(const vector< vector<int> >& stack,
					vector<int> top_coin,
					int bitset) {
	//take the top coins:
	// - check if possible (and there still is a coin)
	// - check if all have the same color, compute score
	vector<int> func_value = top_coin;
	func_value.push_back(bitset);

	//check if we have already computed this function before

	if(memo.find(func_value) != memo.end()) {
		return memo[func_value];
	}


	vector<int> colors_picked;
	for(int i=0; i<stack.size(); i++) {
		//check if i-th bit of 'bitset' is 1 or 0
		int take_coin = (bitset >> i) & 1;
		assert(take_coin == 1 || take_coin == 0);

		
		if(take_coin == 1) {
			if(top_coin[i] == -1) { //trying to take coin from empty stack
				memo.insert(make_pair(func_value, 0));
				return 0;
			}
			else {
				colors_picked.push_back(stack[i][top_coin[i]]);
				top_coin[i]--;
			}
		}
	}

	//check the score of the colors we picked
	int c_size = colors_picked.size();
	assert(c_size >= 1);
	//check that all colors are the same
	int color = colors_picked[0];
	for(int i=1; i<colors_picked.size(); i++) {
		if(color != colors_picked[i]) {
			memo.insert(make_pair(func_value, 0));//not all picked have the same color
			return 0;
		}
	}
	//now that we know all colors are the same 
	//score according to picked amount

	int score = (c_size == 1) ? 0 : pow(2, c_size-2);

	//with updated top coins start new subroutine
	int max_score = 0;

	for(int i=1; i<pow(2, stack.size()); i++) {
		//with i we model which subset of the top coins we take
		//e.g.:
		//00001 would be only coin of stack 1 taken
		//11111 would be top coins of all stacks
		int new_max = maximize_score(stack, top_coin, i);
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

	int max_score = 0;

	for(int i=1; i<pow(2, n_stacks); i++) {
		//with i we model which subset of the top coins we take
		//e.g.:
		//00001 would be only coin of stack 1 taken
		//11111 would be top coins of all stacks
		int new_max = maximize_score(stack, top_coin, i);
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