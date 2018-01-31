#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>
#include "prettyprint.hpp"

using namespace std;

typedef pair<long,long> Interval;

long n_segments;

int edf_shifted(long shift, long end, vector<Interval>& intervals) {
	//first shift all the intervals
	assert(shift < n_segments);
	end = (end + (n_segments - shift)) % n_segments;
	for (int i = 0; i < intervals.size(); ++i)
	{
		//shift into the positive direction so
		//we don't have to deal with negativ modulo
		//'long' should prevent overflows
		long s = intervals[i].first + (n_segments - shift);
		long e = intervals[i].second + (n_segments - shift);
		intervals[i] = make_pair(s % n_segments, e % n_segments);
	}

	//sort by end
	sort(intervals.begin(), intervals.end(), 
		[](const Interval& a, const Interval& b) -> bool{
			if(a.second == b.second)
				return a.first > b.first; //shorter intervals first
			return a.second < b.second; //earliest deadline
		});

	//edf

	assert(intervals.size() > 0);
	vector<Interval> taken;
	taken.push_back(intervals[0]);
	for (int i = 1; i < intervals.size(); ++i)
	{	
	    if(intervals[i].second > end)
	        break;
	        
		if(taken.back().second < intervals[i].first) {
			taken.push_back(intervals[i]);
		}
	}
	//cout << "shift " << shift << " end " << end << endl;
	//cout << "taken "<< taken << endl;
	return taken.size();
}

bool is_contained(int x, Interval& itv) {
    if(itv.first <= itv.second)
        return x >= itv.first && x <= itv.second;
    else
        return x <= itv.first || x >= itv.second;
}


void testcase(){

	int n_jedi;
	cin >> n_jedi >> n_segments;

	vector<Interval> jedi_segment(n_jedi);

	int coversize = 0;
	vector<long> start, end;
	for(int i=0; i<n_jedi; i++){
		long a, b;
		cin >> a >> b;
		if(b < a || b == n_segments) //Interval wraps over 0
			coversize++;

		a = a-1;
		b = b-1; //convert to zero indexing 
		jedi_segment[i] = make_pair(a, b);

		start.push_back(a);
		end.push_back((b+1) % n_segments); //add one so b is the non-inclusive end of the interval
	}

	sort(start.begin(), start.end());
	sort(end.begin(), end.end());

	////cout << start << endl;
	////cout << end << endl;
	//scan for a coversize 0 (later 10)
	int shift = 0;
	vector<long>::iterator is = start.begin(), ie = end.begin();
	while(ie != end.end() && coversize > 10) {
		////cout << "cover " << coversize << " at shift " << shift << endl;
		if(is == start.end() || *ie < *is) {
			shift = *ie;
			coversize--;	
			ie++;
		}
		else if(*is < *ie) {
			shift = *is;
			coversize++;
			is++;
		}
		else { //*ie == *is
			shift = *is;
			ie++;
			is++;
		}
	}
	////cout << "cover " << coversize << " at shift " << shift << endl;
	//cout << "coversize " << coversize << endl;
	
	//compute cover and non-cover intervals	  
	  
	vector<Interval> cover;
	vector<Interval> intervals;  
    for (unsigned int i = 0; i < jedi_segment.size(); i += 1)
    {
        if(is_contained(shift, jedi_segment[i])) {
            cover.push_back(jedi_segment[i]);
        }
        else {
            intervals.push_back(jedi_segment[i]);
        }
    }
    cout << "cover " << cover << endl;
    cout << "intervals " << intervals << endl;
    int count = 0;
    assert(cover.size() <= 10);
	if(cover.size() > 0) {

	    //for each cover interval compute edf
	    for(Interval itv : cover) {
	        count = max(count, edf_shifted(shift, itv.first, intervals) + 1);
	    }
	}
	else {
	    count = edf_shifted(shift, (shift + n_segments - 1) % n_segments, jedi_segment);
	}
	
	cout << count << endl;
}


int main(){
	std::ios_base::sync_with_stdio(false);
	
	int t; cin >> t;

	for(int i=0; i<t; i++){
		testcase();
	}

	return 0;
}

