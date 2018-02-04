#include <iostream>
#include <cassert>
#include <map>
#include <vector>
#include <algorithm>
#include <queue>
#include <set>
#include <unordered_set>
#include <climits>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <tuple>

using namespace std;

int n_parcels;
vector<int> A;
vector<int> B;
vector<int> SA;
vector<int> SB;
unordered_map<tuple<int,int,int,int>,int,boost::hash<tuple<int,int,int,int>>> memo;

int val(int ab, int ae, int bb, int be) {
    return (SA[ae] - SA[ab] - ae + ab)*
           (SB[be] - SB[bb] - be + bb);
}

int solve(int ab, int ae, int bb, int be) {

    if (memo.find(make_tuple(ab, ae, bb, be)) != memo.end()) {
        return memo[make_tuple(ab, ae, bb, be)];
    }

    if (ab == 0 && bb == 0) {
        memo[make_tuple(ab, ae, bb, be)] = 0;
        return memo[make_tuple(ab, ae, bb, be)];
    }
    
    if (ab == 0 || bb == 0) {
        memo[make_tuple(ab, ae, bb, be)] = INT_MAX/2;
        return memo[make_tuple(ab, ae, bb, be)];
    }
    
    if (ae < 0 || be < 0) {
        memo[make_tuple(ab, ae, bb, be)] = INT_MAX/2;
        return memo[make_tuple(ab, ae, bb, be)];
    }
    
    memo[make_tuple(ab, ae, bb, be)] = min({solve(ae, ae-1, be, be-1) + val(ab, ae, bb, be),
                                            solve(ab, ae, bb, be-1),
                                            solve(ab, ae-1, bb, be)});
    return memo[make_tuple(ab, ae, bb, be)];
}

void testcase() {

    cin >> n_parcels;
    
    A = vector<int>(n_parcels);
    for (int i = 0; i < n_parcels; i++) {
        cin >> A[i];
    }
    
    B = vector<int>(n_parcels);
    for (int i = 0; i < n_parcels; i++) {
        cin >> B[i];
    }
    
    SA = vector<int>(n_parcels+1, 0);
    SB = vector<int>(n_parcels+1, 0);
    for (int i = 1; i < n_parcels+1; i++) {
        SA[i] = SA[i-1] + A[i-1];
        SB[i] = SB[i-1] + B[i-1];
    }
    
    memo.clear();
    int res = solve(n_parcels, n_parcels-1, n_parcels, n_parcels-1);
    cout << res << endl;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    //cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
    
    int t; cin >> t;

    for(int i=0; i<t; i++){
        testcase();
    }

    return 0;
}