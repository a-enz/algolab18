#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <cmath>
#include <set>
#include <unordered_map>
#include <boost/functional/hash.hpp>

// Namespaces
using namespace std;

typedef vector< vector<int> >   vecvec;
typedef unordered_map< vector<int>, int, boost::hash< vector<int> > > Hmap;

//global variables
int n_stacks;
vector<int> stacksize;
vector< vector<int> > stack;
Hmap memo;

vector<int> valid_colorset(int bitset, const vector<int>& topcoin) {
    int first_color = -1;
    vector<int> res;
    for (int i = 0; i < n_stacks; ++i)
    {
        if((bitset >> i) & 1) {
            if(topcoin[i] >= 0) {
                first_color = stack[i][topcoin[i]];
                break;
            }
            else {
                return {};
            }
        }
    }

    assert(first_color != -1);
    for (int i = 0; i < n_stacks; ++i)
    {
        if((bitset >> i) & 1) {
            if(topcoin[i] < 0 || first_color != stack[i][topcoin[i]]) {
                return {};
            }
            else 
                res.push_back(i);
        }
    }

    return res;
}

int max_points(vector<int>& topcoin) {

    auto el = memo.find(topcoin);
    if(el != memo.end())
        return el->second;

    //base case
    int invalid_count = 0;
    for (int top : topcoin)
        if(top == -1) invalid_count++;

    if(invalid_count == n_stacks)
        return 0;


    vector<int> results;
    //general case
    for(int bitset = 1; bitset < pow(2, n_stacks); bitset++) {
        vector<int> take = valid_colorset(bitset, topcoin);
        if(not take.empty()) {
            for (int idx : take) {
                    topcoin[idx]--;
            }

            int size = take.size();
            int score = (size == 1) ? 0 : pow(2, size - 2);
            results.push_back(score + max_points(topcoin));

            for (int idx : take) {
                    topcoin[idx]++;
            }
        }
    }

    assert(not results.empty());

    memo[topcoin] = *max_element(results.begin(), results.end());
    return memo[topcoin];
}

void testcases() {
    cin >> n_stacks;

    stacksize = vector<int>(n_stacks);
    for (int i = 0; i < n_stacks; ++i)
    {
        cin >> stacksize[i];
    }

    stack = vecvec(n_stacks);
    vector<int> topcoin = stacksize;
    for (int i = 0; i < n_stacks; ++i)
    {   
        topcoin[i]--;
        stack[i].resize(stacksize[i]);
        for (int j = 0; j < stacksize[i]; ++j)
        {
            cin >> stack[i][j];
        }
    }

    memo.clear();

    cout << max_points(topcoin) << endl;
}

// Main function looping over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;   // First input line: Number of testcases.
    while(T--)  testcases();
    return 0;
}

