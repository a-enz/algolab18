#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <map>
#include <set>
#include "prettyprint.hpp"

// Namespaces
using namespace std;

// Functions
// ========= 
void testcases() {
    int n_words;
    cin >> n_words;

    vector<int> word_count(n_words);
    for (int i = 0; i < n_words; ++i)
    {
        cin >> word_count[i];
    }

    //for each distinct pos store which words occur there
    vector< pair<int, int> > word_pos; // (pos, word)
    for (int i = 0; i < n_words; ++i)
    {
        for (int p = 0; p < word_count[i]; ++p)
        {   
            int pos;
            cin >> pos;
            assert(pos >= 0);
            word_pos.push_back(make_pair(pos, i));
        }
    }

    sort(word_pos.begin(), word_pos.end()); //sort ascending by pos

    //go over the map of positions and note with sliding window
    //noting words that are included in the window
    vector<int> included(n_words, 0); //set of included words
    included[word_pos[0].second]++;//first word
    int total_included = 1;

    //bookkeeping
    vector<int> snippet_length;
    auto left=word_pos.begin(), right=word_pos.begin();
    while(left != word_pos.end()) {
        assert(total_included <= n_words);

        //update possible solutions
        if(total_included == n_words) {
            int dist = right->first - left->first + 1;

            snippet_length.push_back(right->first - left->first + 1);
        }

        //adjust window
        if(total_included < n_words && right < (word_pos.end()-1)) {
            right++;
            if(++included[right->second] == 1)
                total_included++;
        }
        else {
            if(--included[left->second] == 0)
                total_included--;

            left++;
        }
    }

    assert(not snippet_length.empty());
    cout << *min_element(snippet_length.begin(), snippet_length.end()) << endl;
}

// Main function looping over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;   // First input line: Number of testcases.
    while(T--)  testcases();
    return 0;
}

