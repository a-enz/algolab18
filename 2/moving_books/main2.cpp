#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <set>
#include "prettyprint.hpp"

// Namespaces
using namespace std;

// Functions
// ========= 
void testcases() {
    int n_friends, m_boxes;
    cin >> n_friends >> m_boxes;

    //read input
    vector<int> strength(n_friends);
    for (int i = 0; i < n_friends; ++i)
    {
        cin >> strength[i];
    }

    //multiset ordered descending
    multiset<int, std::greater<int> > boxes;

    for (int i = 0; i < m_boxes; ++i)
    {   
        int w;
        cin >> w;
        boxes.insert(w);
    }

    //sort ascending
    sort(strength.begin(), strength.end(), [](const int& a, const int& b) -> bool {
        return a > b;
    });

    //if we find an element strictly larger than the strongest friend the task is impossible
    if(strength[0] < *boxes.begin()) {
        cout << "impossible\n";
        return;
    }

    //cout << boxes << endl;

    //simulate rounds
    int rounds = 0;
    while(not boxes.empty()) {
        //assume 'strength' is sorted in descending order
        //assume 'boxes' is sorted in descending order

        for (int s : strength)
        {   
            //finds first weight equal or less than strength s
            auto b = boxes.lower_bound(s);
            if(b == boxes.end()){ //no box light enough exists
                break;
            }
            else {
                boxes.erase(b);
            }
        }
        rounds++;
    }

    cout << 3*rounds -1 << endl;
}

// Main function looping over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;   // First input line: Number of testcases.
    while(T--)  testcases();
    return 0;
}

