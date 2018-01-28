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
    int costprio;
};

unordered_map< Tuple_2,Tuple_2, boost::hash<Tuple_2 > > memo;

Tuple_2 min_cost_max_bev(int pos, int liters, vector<Bev>& bev) {
    Tuple_2 res = make_tuple(1000000, 0);
     
    Tuple_2 fvalue = make_tuple(pos, liters);
    if(memo.find(fvalue) != memo.end()) 
        return memo[fvalue];
    
    assert(pos >= 0);
    if(pos == bev.size() || liters <= 0) {
        memo[fvalue] = res;
        return memo[fvalue];
    }


    auto comp = [](const Tuple_2& a, const Tuple_2& b)->bool{
                    int c_a, c_b, b_a, b_b;
                    tie(c_a, b_a) = a;
                    tie(c_b, b_b) = b;
                    
                    if(c_a == c_b)
                        return b_a > b_b;
                    else
                        return c_a < c_b;
                };
                
    //take beverage 0 times
    res = min(res, min_cost_max_bev(pos+1, liters, bev), comp);
    
//    //take it multiple times
//    int a = 1;
//    for (; a * bev[pos].vol < liters; a += 1)
//    {
//       int cost, d_bev;
//       tie(cost, d_bev) = min_cost_max_bev(pos+1, liters - a * bev[pos].vol, bev);
//       res = min(res, make_tuple(cost + bev[pos].cost * a, d_bev + 1), comp); 
//       //cout << "updating res " << res << endl;
//    }
    
    //take beverage 1 time
    int cost, d_bev;
    tie(cost, d_bev) = min_cost_max_bev(pos+1, liters - bev[pos].vol, bev);
    res = min(res, , comp);
    
    
    
    
    
    //take it so many times we fill up all the remaining liters
    int amount = 
    res = min(res, make_tuple( * bev[pos].cost, 1), comp); 
    
//    cout << "returning " << res << endl;
    memo[fvalue] = res;
    return memo[fvalue];
}


void testcases() {
    size_t n_bev, k_ppl;
    cin >> n_bev >> k_ppl;
    
    //clear memo;
    memo.clear();
    
    vector<Bev> beverage(n_bev);
    int min_costprio = INT_MAX;
    
    for (unsigned int i = 0; i < n_bev; i += 1)
    {
        cin >> beverage[i].cost;
        cin >> beverage[i].vol;
           
        int mult = k_ppl / beverage[i].vol;
        if(k_ppl % beverage[i].vol != 0) 
            mult += 1;
        
        beverage[i].costprio = beverage[i].cost * mult;
        
        min_costprio = (min_costprio > beverage[i].costprio) ?
                        beverage[i].costprio : min_costprio;
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


