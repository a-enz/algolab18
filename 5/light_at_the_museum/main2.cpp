#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <map>
#include <cmath>

// Namespaces
using namespace std;

typedef vector< vector<int> >   vecvec;
typedef map< vector<int>, int > mapvec;

//global variables
int n_switches, m_rooms;
vecvec off_counts, on_counts;


vector<int> add(const vector<int>& a, const vector<int>& b) {
    //cout << a.size() << " " << b.size() << endl;
    assert(a.size() == b.size());
    vector<int> result(a.size());
    for (int i = 0; i < result.size(); ++i)
    {
        result[i] = a[i] + b[i];
    }
    return result;
}

vector<int> sub(const vector<int>& a, const vector<int>& b) {
    assert(a.size() == b.size());
    vector<int> result(a.size());
    for (int i = 0; i < result.size(); ++i)
    {
        result[i] = a[i] - b[i];
    }
    return result;
}

void generate_list(const int pos, const int end, vector<int>& val, int flips, mapvec& list) {

    if(pos == end) {
        auto el = list.find(val);
        if(el != list.end()) 
            list[val] = min(flips, list[val]);
        else
            list[val] = flips;

        return;
    }

    assert(pos < n_switches);
    //dont' flip the switch
    val = add(val, on_counts[pos]);
    generate_list(pos+1, end, val, flips, list);
    val = sub(val, on_counts[pos]);

    //flip the switch
    val = add(val, off_counts[pos]);
    generate_list(pos+1, end, val, flips+1, list);
    val = sub(val, off_counts[pos]);
}

// Functions
// ========= 
void testcases() {
    cin >> n_switches >> m_rooms;

    vector<int> brightness(m_rooms);
    for (int i = 0; i < m_rooms; ++i)
    {
        cin >> brightness[i];
    }

    off_counts = vecvec(n_switches, vector<int>(m_rooms));
    on_counts = vecvec(n_switches, vector<int>(m_rooms));
    for (int s = 0; s < n_switches; ++s)
    {
        for (int r = 0; r < m_rooms; ++r)
        {
            cin >> on_counts[s][r] >> off_counts[s][r];
        }
    }

    //compute the lists
    mapvec list1, list2;
    vector<int> ze1(m_rooms, 0), ze2(m_rooms, 0);
    generate_list(0, n_switches / 2, ze1, 0, list1);

    generate_list(n_switches / 2, n_switches, ze2, 0, list2);

    int min_switches = n_switches+1;
    for(auto it = list1.begin();
        it != list1.end();
        it++) {

        vector<int> missing = sub(brightness, it->first);
        auto other = list2.find(missing);
        if(other != list2.end()) {
            min_switches = min(min_switches, it->second + other->second);
        }
    }

    if(min_switches < n_switches+1)
        cout << min_switches << endl;
    else
        cout << "impossible\n";

}

// Main function looping over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;   // First input line: Number of testcases.
    while(T--)  testcases();
    return 0;
}

