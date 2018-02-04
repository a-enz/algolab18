#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>
#include <cassert>
#include "prettyprint.hpp"
#include <unordered_map>
#include <boost/functional/hash.hpp>

// Namespaces
using namespace std;

typedef unordered_map< vector<int>, int, boost::hash< vector<int> > > Hmap;
typedef vector< vector< vector< vector<int> > > > Vmap; 

vector<int> b_ps, a_ps;
Vmap memo;

int min_cost(int a, int i, int k1, int k2) {
    //check memo
    if(memo[a][i][k1][k2] != -1)
    return memo[a][i][k1][k2];

    if(i == k2 && i+1 == a_ps.size()) {
        int val;
        if(a==1) {
            val = (a_ps[i] - a_ps[i-1] - 1) * 
                (b_ps[k2] - b_ps[k1-1] - (k2-k1 + 1));
        }
        else {
            assert(a==0);
            val = (b_ps[i] - b_ps[i-1] - 1) * 
                (a_ps[k2] - a_ps[k1-1] - (k2-k1 + 1));
        }

        memo[a][i][k1][k2] = val;
        return memo[a][i][k1][k2];
    }
    else if(i != k2 && (i == a_ps.size() || k2 == a_ps.size())) {
        memo[a][i][k1][k2] = pow(10, 9); //basically inf
        return memo[a][i][k1][k2];
    }


    //general case
    int opt1, opt2;
    if(a==1) {
        int val = (a_ps[i] - a_ps[i-1] - 1) * 
                (b_ps[k2] - b_ps[k1-1] - (k2-k1 + 1));
        opt1 = val + min_cost(a, i+1, k2+1, k2+1);
        opt2 = val + min_cost(1-a, k2+1, i+1, i+1);
    }
    else {
        assert(a==0);
        int val = (b_ps[i] - b_ps[i-1] - 1) * 
                (a_ps[k2] - a_ps[k1-1] - (k2-k1 + 1));

        opt1 = val + min_cost(a, i+1, k2+1, k2+1);
        opt2 = val + min_cost(1-a, k2+1, i+1, i+1);
    }

    int opt3 = min_cost(a, i, k1, k2+1);

/*    cout << "options " << opt1 << " " << opt2 << " " << opt3 << " " << opt4 << endl;
*/   memo[a][i][k1][k2] = min({opt1, opt2, opt3});
    return memo[a][i][k1][k2];
}



void testcases() {
    int n_parcels;
    cin >> n_parcels;

    memo = Vmap(2, vector< vector< vector<int> > >(n_parcels+2,
                            vector< vector<int> >(n_parcels+2,
                            vector<int>(n_parcels+2, -1))));

    b_ps = vector<int>(n_parcels+1); 
    a_ps = vector<int>(n_parcels+1);
    vector<int> a(n_parcels), b(n_parcels);
    for (int i = 0; i < n_parcels; ++i)
    {
        cin >> a[i];
    }

    for (int i = 0; i < n_parcels; ++i)
    {
        cin >> b[i];
    }

    b_ps[0] = 0;
    a_ps[0] = 0;
    for (int i = 1; i < n_parcels+1; ++i)
    {
        a_ps[i] = a_ps[i-1] + a[i-1];
        b_ps[i] = b_ps[i-1] + b[i-1];
    }

/*    cout << a_ps << endl << b_ps << endl;
*/

    cout << min_cost(1, 1, 1, 1) << endl;


}

// Main function looping over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;   // First input line: Number of testcases.
    while(T--)  testcases();
    return 0;
}

