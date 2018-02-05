#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <cmath>
#include <set>
#include "prettyprint.hpp"

using namespace std;

typedef vector< vector<int> > MSV;

//global variables
int n_planks;
vector<int> length;
int side_length;

void generate_list(int start, int end, vector<int>& val, MSV& list) {

    if(start == end) {
        list.push_back(val);
        return;
    }

    //general case: add current to one of the four sides
    for (int i = 0; i < 4; ++i)
    {
        val[i] += length[start];
        generate_list(start+1, end, val, list);
        val[i] -= length[start];
    }
}

void testcases() {  
    cin >> n_planks;

    length = vector<int>(n_planks);
    int total_lenth = 0;
    int max_len = 0;
    for (int i = 0; i < n_planks; ++i)
    {
        cin >> length[i];
        total_lenth += length[i];
        max_len = max(length[i], max_len);
    }

    side_length = total_lenth / 4;

    if(total_lenth % 4 != 0 || max_len > side_length) {
        cout << "0\n";
        return;
    }

    vector<int> val1(4, 0), val2(4, 0);
    MSV list1, list2;
    list1.reserve(pow(2, n_planks / 2));
    list2.reserve(pow(2, n_planks / 2));
    generate_list(0, n_planks / 2, val1, list1);
    generate_list(n_planks / 2, n_planks, val2, list2);

    sort(list2.begin(), list2.end());

    long total_perm = 0;
    for(vector<int> element : list1) {
        vector<int> inv(4, side_length);
        for (int i = 0; i < 4; ++i)
        {
            inv[i] -= element[i];
        }

        auto lower = lower_bound(list2.begin(), list2.end(), inv);
        auto upper = upper_bound(list2.begin(), list2.end(), inv);

        if(lower != upper)
            total_perm += distance(lower, upper); 
    }

    cout << total_perm / 24 << endl;
}

// Main function looping over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;   // First input line: Number of testcases.
    while(T--)  testcases();
    return 0;
}

