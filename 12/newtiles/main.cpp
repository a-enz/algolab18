#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include "prettyprint.hpp"

using namespace std;

// Functions
// ========= 
void testcases() {
    int h_height, w_width;
    cin >> h_height >> w_width;
    
    vector< vector<int> > plan(h_height, vector<int>(w_width));
    for (int i = 0; i < h_height; i += 1)
    {
        for (int j = 0; j < w_width; j += 1)
        {
            cin >> plan[i][j];
        }
    }
    
    //handle special case
    if(h_height < 4 || w_width < 4) {
        cout << "0\n";
        return;
    }
    
    //convert plan to rows of bitstrings
    vector<int> rows(h_height-2, 0);
    for (int i = 1; i < h_height-1; i += 1)
    {   
        int val = 0;
        for (int j = 1; j < w_width-1; j += 1)
        {
            int shift = w_width-2-j;
            val += plan[i][j] << shift;
        }
        rows[i-1] = val;
    }
    
//    cout << rows << endl;
    
    int height_dp = h_height-2;
    int width_dp = pow(2, w_width-2);
    vector< vector<int> > table(height_dp, vector<int>(width_dp, 0));
    
    //precompute for each bitmask if it contains even number of consecutive ones
    vector<bool> is_candidate(width_dp, true);
    vector<int> amount_ones(width_dp, 0);
    is_candidate[0] = false;
    for (int bitmask = 1; bitmask < width_dp; bitmask += 1)
    {
        for (int pos = 0; pos < w_width-2; pos += 1)
        {
            if((bitmask >> pos) & 1) {//current pos has 1
                if((pos+1 == w_width-2) || not (bitmask >> (pos+1) & 1)) {//next pos has not a 1 
                    is_candidate[bitmask] = false;
                    break;
                }
                else { // next pos has consecutive 1, skip
                    pos++;
                }
            }
        }
    }
    
                    
    //count ones in this bitmask
    for (int bitmask = 1; bitmask < width_dp; bitmask += 1)
    {
        for (int pos = 0; pos < w_width-2; pos += 1)
        {
            if((bitmask >> pos) & 1) {//current pos has 1
                amount_ones[bitmask]++;
            }
        }
    }
    
//    cout << is_candidate << endl;
//    cout << amount_ones << endl;
    
    //fill dp table
    const int ALL_ONES_MASK = width_dp-1;
    
    for (unsigned int i = 1; i < table.size(); i += 1)
    {
        for (int bitmask = 0; bitmask < pow(2, w_width-2); bitmask += 1)
        {   
//            cout << "mask " << bitmask << "size " << table.at(i).size() << endl;
            assert(bitmask < table[i].size());
            assert(bitmask <= ALL_ONES_MASK);
            int opt1;
            if(is_candidate[bitmask] && 
                ((bitmask & rows[i]) == bitmask) &&
                ((bitmask & rows[i-1]) == bitmask)) {
                //placement of tiles makes sense for this bitmask
                //because it has even number of consecutive 1
                //and row[i] and row[i-1] allows placement as bitmask suggests
                //cout << bitmask << endl;
                assert(amount_ones[bitmask] % 2 == 0);
                int inverted_bitmask = ALL_ONES_MASK ^ bitmask;
                assert(inverted_bitmask < width_dp);
                opt1 = table[i-1][inverted_bitmask] + amount_ones[bitmask] / 2;
            }
            else {
                opt1 = 0;
            }
            
            int opt2 = 0;
            for (int pos = 0; pos < w_width-2; pos += 1)
            {
                if((bitmask >> pos) & 1) {
                    //flip bitmask bit at 'pos'
                    int bitmask_other = bitmask ^ (1 << pos);
                    assert(bitmask_other < bitmask);
                    opt2 = max(opt2, table[i][bitmask_other]);
                }
            }
            
            table[i][bitmask] = max(opt1, opt2);
        }
    }
    
    
//    cout << "table\n" << endl;
//    for (int i = 0; i < table.size(); i += 1)
//    {
//        cout << table[i] << endl;
//    }
    
    //read solution from table
    cout << table[height_dp-1][width_dp-1] << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


