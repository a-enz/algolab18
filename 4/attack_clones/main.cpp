#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <map>
#include <set>
#include "prettyprint.hpp"

using namespace std;

typedef pair<long,long> Interval;
typedef map<int, vector<Interval> > Imap;

long n_segments;

int edf_shifted(int shift, vector<Interval>& intervals) {
	//first shift all the intervals
	assert(shift < n_segments);
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
		if(taken.back().second < intervals[i].first) {
			taken.push_back(intervals[i]);
		}
	}
	return taken.size();
}


void testcase(){

	int n_jedi;
	cin >> n_jedi >> n_segments;

	vector<Interval> jedi_segment(n_jedi);

	multiset<Interval> cover;
	Imap start, end;
	for(int i=0; i<n_jedi; i++){
		long a, b;
		cin >> a >> b;

		int a0 = a-1; 
		int b0 = b % n_segments; //zero indexed, non-inclusive interval borders
		jedi_segment[i] = make_pair(a-1, b-1); //zero indexing
		if(b < a) //Interval wraps over 0
			cover.insert(jedi_segment[i]);

		auto si = start.find(a0), se = end.find(b0);
		if(si == start.end()) 
			start[a0] = {jedi_segment[i]};
		else
			si->second.push_back(jedi_segment[i]);

		if(se == end.end()) 
			end[b0] = {jedi_segment[i]};
		else
			se->second.push_back(jedi_segment[i]);
	}

/*	cout << cover << endl;
	cout << start << endl;
	cout << end << endl;*/
	int max_segments = 0;
	if(cover.size() > 0) {
		//scan for a coversize 1 (later 10)
		Imap::iterator ie = end.begin(), is = start.begin();
		while(ie != end.end() && cover.size() > 1) {
			//cout << "cover " << coversize << " at shift " << shift << endl;
			if(is == start.end() || *ie < *is) {
				//remove intervals from cover
				for (Interval itv : ie->second)
					cover.erase(itv);

				ie++;
			}
			else if(*is < *ie) {
				for (Interval itv : is->second)
					cover.insert(itv);

				is++;
			}
			else { //*ie == *is
				for (Interval itv : is->second)
					cover.insert(itv);
				for (Interval itv : ie->second)
					cover.erase(itv);

				ie++;
				is++;
			}
		}


		for(Interval itv : cover) {
			int new_max = edf_shifted(itv.first, jedi_segment);
			max_segments = max(new_max, max_segments);
		}
	}
	else {
		max_segments = edf_shifted(0, jedi_segment);
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
