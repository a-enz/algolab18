#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>

// Namespaces
using namespace std;

// Functions
// ========= 
void testcases() {
    int n_disks, i_con, k_mod;
    cin >> n_disks >> i_con >> k_mod;
    
    vector<int> height(n_disks);
    for (int i = 0; i < n_disks; i += 1)
    {   
        long h;
        cin >> h;
        height[i] = h % k_mod;
    }

    vector< vector<bool> > table(n_disks+1, vector<bool>(k_mod));
    
    table[0] = vector<bool>(k_mod, false);
    for (int item = 1; item <= n_disks; item += 1)
    {
        for (int j = 0; j < k_mod; j += 1)
        {
            int inverse = (k_mod - ((height.at(item-1) + j) % k_mod)) % k_mod;
//            cout << "inverse " << inverse << endl;
            assert(inverse >= 0);
            assert(inverse < k_mod);
        
            table[item][j] = (height.at(item-1) % k_mod == j) || //item itself is congruent
                               //there exists a subset that has inverse to congruence of item
                               table[item-1][inverse] ||           
                               table[item-1][j]; //congruence to j is possible with smaller subset
        }
    }
    
    if(table[n_disks][i_con])
        cout << "yes\n";
    else
        cout << "no\n";
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


