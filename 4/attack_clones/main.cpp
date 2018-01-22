#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <climits>
#include <cassert>
#include <unordered_map>
#include "prettyprint.hpp"
//#include <boost/functional/hash.hpp>

using namespace std;

typedef pair<int, int> Interval;

void testcase() {
	int n_jedi, m_seg;
	cin >> n_jedi >> m_seg;

    vector<Interval> segment(n_jedi);
    for (int i = 0; i < n_jedi; ++i)
    {
        int a, b;
        cin >> a >> b;
        a--; //convert to 0-indexing
        b--;
        if(b < a) //convert to 2*m linear array, hopefully less hassle than wrap around
            b = m_seg + b;

        assert(a < m_seg);
        //if b < a then a could at most be (m-1) thus b at most (m-2) 
        //after converting to 0 indexing
        assert(b < 2*m_seg - 1);

        segment[i] = make_pair(a, b); //convert to zero indexing
    }

    //create a map: seg -> (#starting, #ending)
    vector<int> segment_starts(2*m_seg, 0);
    vector<int> segment_ends(2*m_seg, 0);

    for (int i = 0; i < n_jedi; ++i)
    {
        int start = segment[i].first;
        int end = segment[i].second;
        segment_starts[start]++;
        segment_ends[end+1]++; // interval is inclusive, thus end is one after
    }

    vector<int> covered(2*m_seg, 0); //count how many intervals cover segments
    //cover of segment i in the circle is 'covered[i] + covered[(m+i)]'
    int coversize = 0;
    for (int i = 0; i < 2*m_seg; ++i)
    {
        coversize += segment_starts[i] - segment_ends[i];
        assert(coversize >= 0);
        covered[i] = coversize;
    }

    //find segment of coversize 0 (or 10 for later i guess?)
    int shift_idx = -1;
    for (int i = 0; i < m_seg; ++i)
    {
        if(covered[i] + covered[i+m_seg] <= 0) { //replace by 10 later??
            shift_idx = i;
            break;
        }
    }

    if(shift_idx == -1) {
        cout << "???\n";
        return;
    }

    //now that we know about a uncoverd segment we can do 
    //greedy earliest deadline first starting from that interval
    //because we are guaranteed no wrap around

    //shift all segments
    vector<Interval> shifted_segments = segment;
    
    for (int i = 0; i < n_jedi; ++i)
    {
        shifted_segments[i].first = (shifted_segments[i].first - (shift_idx+1)) % (2 * m_seg);
        shifted_segments[i].second = (shifted_segments[i].second - (shift_idx+1)) % (2 * m_seg);

        //ensure no negative modulo
        if(shifted_segments[i].first < 0)
            shifted_segments[i].first += 2*m_seg;
        if(shifted_segments[i].second < 0)
            shifted_segments[i].second += 2*m_seg;
    }
            

/*
    cout << covered << endl;
    cout << "shift " << shift_idx+1 << endl;
    for (int i = 0; i < n_jedi; ++i)
    {  
        cout << "shift " << shifted_segments[i].first << " shift " << shifted_segments[i].second << endl;
        cout << "start " << segment[i].first << " ende " << segment[i].second << endl;
    }
*/

    //do EDF on shifted
    sort(shifted_segments.begin(), shifted_segments.end(), 
        [](const Interval& i1, const Interval& i2) -> bool {
            if(i1.second == i2.second)
                return i1.first < i2.first;
            else
                return i1.second < i2.second;
        });
        
    //cerr << "segments " << segment << endl;
    //cerr << "shifted segments "  << shifted_segments << endl;
    

    int jcounter = 1;
    bool stop = false;
    Interval prev = shifted_segments[0];
    //cerr << "picking " << prev.first << ", " << prev.second << endl;
    for (int i = 1; i < n_jedi; ++i)
    {   
        //while the next 'a' (start)
        //id before or equal to previous 'b' (end)
        //discard
        while(prev.second % m_seg >= shifted_segments[i].first % m_seg) {
            if(i > n_jedi-1) {
                stop = true;
                break;
            }
            i++;
        }
        if(stop) break;
        prev = shifted_segments[i];
        //cerr << "picking " << prev.first << ", " << prev.second << endl;
        jcounter++;
    }

    cout << jcounter << endl;
}

int main(void) {

	//disable for performance
	ios_base::sync_with_stdio(false);
	//read total number of test cases
	int n_total_testcases;
	cin >> n_total_testcases;

	//do for each testcase
	for(int n_test = 0; n_test < n_total_testcases; n_test++) {
		testcase();
	}

	return 0;
}
