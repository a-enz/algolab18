#include <iostream>
#include <vector>
#include <assert.h>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include "prettyprint.hpp"
#include <set>

using namespace std;

unordered_map< vector<int>, int, boost::hash< vector<int> > > memo;

set<int> color_candidates;

bool subset_valid(const vector< vector<int> >& stack,
                    const vector<int>& top_coin, 
                    const int bitset,
                    vector<int>& take) {

    int first_color = -1;
    for (int i = 0; i < stack.size(); ++i)
    {
        if((bitset >> i) & 1) {
            first_color = stack[i].at(top_coin[i]);
            break;
        }
    }

    assert(first_color > 0);
    assert(first_color <= pow(2, 10));

    for (int i = 0; i < stack.size(); ++i)
    {
        if((bitset >> i) & 1) {
            if(first_color != stack[i].at(top_coin[i])) {
                return false;
            }
            else {
                take.push_back(i);
            }
        }
        assert(top_coin[i] >= 0);
    }

    return true;
}


int maximize_score(const vector< vector<int> >& stack,
					vector<int>& top_coin) {
    assert(top_coin.size() == stack.size());
    //check memo
    vector<int> fvalues = top_coin;
    auto result = memo.find(fvalues);
    if(result != memo.end())
        return result->second;

    //check if there is a invalid top_coin position
    for (int i = 0; i < top_coin.size(); ++i)
    {
        if(top_coin[i] < 0) {

            assert(memo.find(fvalues) == memo.end());
            memo[fvalues] = 0;
            return memo[fvalues];
        }
    }

    //go over all possible subset, check if it is valid color subset
    int max_score = 0;
    for (int bitset = 1; bitset < pow(2, stack.size()); ++bitset)
    {   
        vector<int> take;
        if(subset_valid(stack, top_coin, bitset, take)) {
            //open recursion
            assert(take.size() > 0);
            int color = stack.at(take[0]).at(top_coin.at(take[0]));
            for (int k : take)
            {   
                assert(color == stack.at(k).at(top_coin.at(k)));
                top_coin[k]--;
            }

            if(take.size() >= 2)
                color_candidates.insert(color);

            int score = (take.size() == 1) ? 0 : pow(2, take.size()-2);
            int val = maximize_score(stack, top_coin);

            max_score = std::max(max_score, val + score);

            for (int k : take)
            {
                top_coin[k]++;
            }
        }
    }

    assert(memo.find(fvalues) == memo.end());

    memo[fvalues] = max_score;
    assert(memo[fvalues] >= 0);
    return memo[fvalues];

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
    color_candidates.clear();

	//read values of chips
	for(int s=0; s<n_stacks; s++) {
        stack[s].reserve(stacksize[s]);
		for(int i=0; i<stacksize[s]; i++) {
			int val;
			cin >> val;
			stack[s].push_back(val);
		}
	}

    int res = maximize_score(stack, top_coin);
    //cout << color_candidates << endl;

	cout << res << endl;

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