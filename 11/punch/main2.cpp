#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>
#include <cassert>

using namespace std;

typedef vector< vector< pair<int, int> > > Vmap;

int n_bev, k_lit;
vector<int> cost, vol;
Vmap memo;

inline pair<int, int> operator+(const pair<int, int>& a, const pair<int, int>& b) {
    return make_pair(a.first+b.first, a.second+b.second);
}

pair<int, int> mincost_maxbev(int pos, int lit) {

    //base cases
    if(lit >= k_lit)
        return make_pair(0, 0);

    if(pos >= n_bev)
        return make_pair(pow(10, 8), 0); //INF COST


    if(memo[pos][lit] != make_pair(-1, -1))
        return memo[pos][lit];


    //general case
    //0) dont't take the beverage
    pair<int, int> opt1, opt2, opt3;
    opt1 = mincost_maxbev(pos+1, lit);
    //1) take the bev and consider taking it again
    opt2 = make_pair(cost[pos], 0) + mincost_maxbev(pos, lit + vol[pos]);
    //3) take the bev and move on, increasing the unique count
    opt3 = make_pair(cost[pos], -1) + mincost_maxbev(pos + 1, lit + vol[pos]);

    memo[pos][lit] = min({opt1, opt2, opt3});
    return memo[pos][lit];
}

// Functions
// ========= 
void testcases() {
    cin >> n_bev >> k_lit;

    memo = Vmap(n_bev, vector< pair<int, int> >(k_lit, make_pair(-1, -1)));

    cost = vector<int>(n_bev);
    vol = vector<int>(n_bev);
    for (int i = 0; i < n_bev; ++i)
    {
        cin >> cost[i] >> vol[i];
    }

    pair<int, int> res = mincost_maxbev(0, 0);

    cout << res.first << " " << -1 * res.second  << endl;
}

// Main function looping over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;   // First input line: Number of testcases.
    while(T--)  testcases();
    return 0;
}

