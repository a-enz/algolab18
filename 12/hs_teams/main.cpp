#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <cmath>
#include <set>
// Namespaces
using namespace std;

typedef vector< vector<int> > vecvec;

vector<int> students;
int k_ref;


void generate_list(int pos, int end, vector<int>& values, vecvec& list) {
    //'values[0]' contains the current difference to 0
    //      players going into team 0: are (+) added to the difference
    //      players going into team 1: are (-) subtracted from the difference
    //'values[1]' contains how many players are not participating
    assert(values.size() == 2);

    //base case, insert the computation
    if(pos == end) {
        list.push_back(values);
        return;
    }

    assert(pos < end);
    //general case, compute further
    //0) add to first team:
    values = {values[0] + students[pos], values[1]};
    generate_list(pos+1, end, values, list);
    values = {values[0] - students[pos], values[1]};
    //1) add to second team
    values = {values[0] - students[pos], values[1]};
    generate_list(pos+1, end, values, list);
    values = {values[0] + students[pos], values[1]};
    //if still possible, add to no team
    if(values[1] < k_ref) {
        values = {values[0], values[1] + 1};
        generate_list(pos+1, end, values, list);
        values = {values[0], values[1] - 1};
    }
}

// Functions
// ========= 
void testcases() {
    int n_students;
    cin >> n_students >> k_ref;
    
    students = vector<int>(n_students);
    for (int i = 0; i < n_students; ++i)
    {
        cin >> students[i];
    }

    vecvec list1, list2;
    vector<int> ze1 = {0, 0};
    vector<int> ze2 = ze1;
    generate_list(0, n_students / 2, ze1, list1);
    generate_list(n_students / 2, n_students, ze2, list2);

    sort(list2.begin(), list2.end());

    long total_config = 0;
    for (auto val : list1)
    {   
        assert(val[1] <= k_ref);
        // for 'val[0]' find a values such that they add up to 0
        // for 'val[1]' we need a value that is <- [0, ..., k - val[1]]
        //    so that they add up to k_ref
        vecvec::iterator upper, lower;
        vector<int> v1 = {-val[0], 0};
        vector<int> v2 = {-val[0], k_ref - val[1]};
        lower = lower_bound(list2.begin(), list2.end(), v1);
        upper = upper_bound(list2.begin(), list2.end(), v2);
        if(lower != upper) {

            //found a range of values that fit
            int range = distance(lower, upper);
            total_config += range;
        }
    }

    cout << total_config << endl;
}

// Main function looping over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;   // First input line: Number of testcases.
    while(T--)  testcases();
    return 0;
}

