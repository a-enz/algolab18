// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <tuple>
#include <cassert>
#include <cmath>
#include "prettyprint.hpp"
#include <unordered_map>
#include <boost/functional/hash.hpp>

using namespace std;

typedef tuple<int, int> Tuple_2;

struct Bev {
    int cost;
    int vol;
};

//unordered_map< Tuple_2,Tuple_2, boost::hash<Tuple_2 > > memo;
vector< vector<Tuple_2> > memo;

Tuple_2 min_cost_max_bev(int pos, int liters, vector<Bev>& bev) {
    Tuple_2 res = make_tuple(1000001, 0);
    
    assert(pos >= 0);
    
    //base case
    if(liters <= 0) {
        return make_tuple(0, 0);
    }
    else if(pos == bev.size()) {
        return res;
    }
    

    //cout << "l " << liters << " s " << memo[pos].size() << endl;
    //assert(liters < (int) memo[pos].size());
    
    
    
    //check memo
    int cost, d_bev;
    tie(cost, d_bev) = memo[pos][liters];
    if(cost != -1 && d_bev != -1) 
        return memo[pos][liters];


    auto comp = [](const Tuple_2& a, const Tuple_2& b)->bool{
                    int c_a, c_b, b_a, b_b;
                    tie(c_a, b_a) = a;
                    tie(c_b, b_b) = b;
                    
                    if(c_a == c_b)
                        return b_a > b_b;
                    else
                        return c_a < c_b;
                };
    //general case      
    //1) take beverage 0 times
    res = min(res, min_cost_max_bev(pos+1, liters, bev), comp);
    
    //2) take beverage 1 time (increase diversity) and move on
    tie(cost, d_bev) = min_cost_max_bev(pos+1, liters - bev[pos].vol, bev);
    res = min(res, make_tuple(cost + bev[pos].cost, d_bev + 1) , comp);
    
    //3) take beverage 1 time, stay on the same position (not increasing diversity)
    tie(cost, d_bev) = min_cost_max_bev(pos, liters - bev[pos].vol, bev);
    res = min(res, make_tuple(cost + bev[pos].cost, d_bev) , comp);


    memo[pos][liters] = res;
    return memo[pos][liters];
}


void testcases() {
    size_t n_bev, k_ppl;
    cin >> n_bev >> k_ppl;
    
    //clear memo;
    memo = vector< vector< tuple<int, int> > >
                    (n_bev, vector< tuple<int, int> >(k_ppl+1, make_tuple(-1, -1)));
    
    vector<Bev> beverage(n_bev);
    
    for (unsigned int i = 0; i < n_bev; i += 1)
    {
        cin >> beverage[i].cost;
        cin >> beverage[i].vol;
    }

    
    int min_cost, max_bev;
    tie(min_cost, max_bev) = min_cost_max_bev(0, k_ppl, beverage);

    cout << min_cost << " " << max_bev << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


