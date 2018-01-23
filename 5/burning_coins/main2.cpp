#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

// Namespaces
using namespace std;

vector< vector<int> > memo;

int max_winnings(int first, int last, const vector<int>& value) {

    //base case
    if(first > last){
        return 0;
    }


    if(memo[first][last] != -1)
        return memo[first][last];

    //take first, assume the worst. Thus compute minimum of the two options oponent has
    int opt1 = value[first] + min(max_winnings(first+2, last, value),  
                                max_winnings(first+1, last-1, value));

    int opt2 = value[last] + min(max_winnings(first, last-2, value),  
                                max_winnings(first+1, last-1, value));

    memo[first][last] = max(opt1, opt2);
    return memo[first][last];
}

// Functions
// ========= 
void testcases() {
    int n_coins;
    cin >> n_coins;

    memo = vector< vector<int> >(n_coins, vector<int>(n_coins, -1));

    vector<int> value(n_coins);
    for (int i = 0; i < n_coins; ++i)
    {
        cin >> value[i];
    }

    cout << max_winnings(0, n_coins-1, value) << endl;
     
}

// Main function looping over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;   // First input line: Number of testcases.
    while(T--)  testcases();
    return 0;
}

