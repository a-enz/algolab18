#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>
#include "prettyprint.hpp"

using namespace std;

typedef pair<long,long> Interval;

long n_segments;

int edf(int idx, vector<Interval>& intervals) {
    
    int end = intervals[idx].first + n_segments;
    vector<Interval> taken;
	taken.push_back(intervals[idx]);
	idx++;	
    for (; idx < intervals.size() && intervals[idx].second < end; idx += 1)
    {
		if(taken.back().second < intervals[idx].first) {
			taken.push_back(intervals[idx]);
		}
    }
    return taken.size();
}


bool contains_point(long x, const Interval& itv) {
    return x >= itv.first && x <= itv.second;
}


void testcase(){

	int n_jedi;
	cin >> n_jedi >> n_segments;

	vector<Interval> jedi_segment;
	jedi_segment.reserve(3*n_jedi);

	int coversize = 0;
	//store start and endpoints in the same list
	vector< pair<long, int> > relevant;
	for(int i=0; i<n_jedi; i++){
		long a, b;
		cin >> a >> b;
	    a--;
		b--; //convert to zero indexing 
		if(b < a) {//Interval wraps around
			coversize++;
			jedi_segment.push_back(make_pair(a, n_segments + b));
			jedi_segment.push_back(make_pair(n_segments + a, 2*n_segments + b));
	    }
	    else {
	        jedi_segment.push_back(make_pair(a, b));
			jedi_segment.push_back(make_pair(n_segments + a, n_segments + b));
			jedi_segment.push_back(make_pair(2*n_segments + a, 2*n_segments + b));
	    }
		
        //starts, be sure that start pairs are lexicographically before end pairs, thus (a, 0)
        //this ensure the coversize in a segment where several intervals start and stop
        //is not first decreased and then increased, thus giving a wrong minimum
		relevant.push_back(make_pair(a, 0));
		//end
		relevant.push_back(make_pair(b, 1));
	}
    
    sort(jedi_segment.begin(), jedi_segment.end(), 
        [](const Interval& a, const Interval& b)->bool{
            return a.second < b.second; //sort by earliest end
        });
	sort(relevant.begin(), relevant.end());

	//scan for smallest coversize
	int shift = 0;
	int min_coversize = coversize;
	for(pair<long, int> r : relevant) {
        if(r.second == 0) {
            coversize++;
        }
        else {
            assert(r.second == 1);
            coversize--;
            if(coversize < min_coversize) {
                min_coversize = coversize;
                shift = r.first + 1; //non-inclusive end of the interval
            }
        }
	}
	
	shift += n_segments; //search in [m, 3m]
	
	int max_segments = 0;
	if(min_coversize == 0) {
	    //find first segment beginning after shift and do edf
	    for (unsigned int i = 0; i < jedi_segment.size(); i += 1)
	    {
	        if(jedi_segment[i].first > shift) {
	            max_segments = edf(i, jedi_segment);
	            break;
	        }      
	    }
	}
	else {
        //find the intervals that contain the shift and start an edf from there
        int count = 0;
        for (unsigned int i = 0; i < jedi_segment.size(); i += 1)
        {
            if(contains_point(shift, jedi_segment[i])) {
                count++;
                max_segments = max(max_segments, edf(i, jedi_segment));
            }
        }    
        cout << "count " << count << endl;
        assert(count == min_coversize);
    }

    
    cout << max_segments << endl;
}


int main(){
	std::ios_base::sync_with_stdio(false);
	
	int t; cin >> t;

	for(int i=0; i<t; i++){
		testcase();
	}

	return 0;
}

