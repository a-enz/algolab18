#include <iostream>
#include <vector>
#include <assert.h>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include "prettyprint.hpp"

using namespace std;

unordered_map< vector<int>, int, boost::hash< vector<int> > > memo;

int maximize_score(const vector< vector<int> >& stack,
					vector<int>& top_coin) {

    //check memo
    auto result = memo.find(top_coin);
    if(result != memo.end())
        return result->second;

    //go over all possible subset, check if it is valid color subset
    int max_score = 0;
    for (int bitset = 1; bitset < pow(2, stack.size()); ++bitset)
    {   
        bool valid_set = true;
        int set_color = -1;

        vector<int> take;
        for (int i = 0; i < stack.size(); ++i)
        {
            if((bitset >> i) & 1) {
                if(top_coin[i] < 0 || 
                    (set_color != -1 && stack[i][top_coin[i]] != set_color)) {
                    valid_set = false;
                    break;
                }
                else {
                    set_color = stack[i][top_coin[i]];
                    take.push_back(i);
                }
            }
        }


        if(valid_set) {
            //open recursion
            assert(take.size() > 0);
            int color = stack.at(take[0]).at(top_coin.at(take[0]));
            for (int k : take)
            {   
                assert(color == stack.at(k).at(top_coin.at(k)));
                top_coin[k]--;
            }

            int score = (take.size() == 1) ? 0 : pow(2, take.size()-2);
            int val = maximize_score(stack, top_coin);

            max_score = std::max(max_score, val + score);

            for (int k : take)
            {
                top_coin[k]++;
            }
        }
    }

    memo[top_coin] = max_score;
    return memo[top_coin];

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
	memo.clear();

	//read values of chips
	for(int s=0; s<n_stacks; s++) {
        stack[s].reserve(stacksize[s]);
		for(int i=0; i<stacksize[s]; i++) {
			int val;
			cin >> val;
			stack[s].push_back(val);
		}
	}

	cout << maximize_score(stack, top_coin) << endl;
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