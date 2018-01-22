#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>

using namespace std;



typedef pair<int,int> P;

//earliest deadline
struct compareFunc
{
    bool operator()( P a, P b)
    {
        return a.second > b.second;
    }
};

typedef priority_queue<P, vector<P>, compareFunc> Q;

//corresponds to b-a in mod k
int circ_diff(int a, int b, int k){
	int res = b - a;
	if(res < 0) return res + k;
	else return res;
}

//transforms the circular interval into linear
Q transform_interval_and_sort(vector<P> v, int starting_point, int k){

	Q res;
	int start, end;

	for(int i=0; i<(int) v.size(); i++){

		start = circ_diff(starting_point, v[i].first, k);
		end = circ_diff(starting_point, v[i].second, k);

		if(end < start) end += k;

		res.push(P (start, end));
	}

	return res;
}

void testcase(){

	int n_jedi, n_segments; cin >> n_jedi >> n_segments;

	vector<P> jedi_segment(n_jedi);
	Q jedi_queue;

	int a, b;

	for(int i=0; i<n_jedi; i++){

		cin >> a >> b;
		jedi_segment[i] = P (a, b);
	}

	//algo start

	//search for shortest interval -> gets to be the start

	P starting_segment = *min_element(jedi_segment.begin(), jedi_segment.end(), [](P a, P b){return abs(a.first-a.second) < abs(b.first-b.second);});

	//sort according to earliest deadline

	jedi_queue = transform_interval_and_sort(jedi_segment, starting_segment.second, n_segments);

	//loop through queue
	//if top element has overlapping start -> discard
	//if else res++
	//check for cycleback of original segment structure

	P current;
	int occupied_pos = 0;
	int res = 1;

	while(!jedi_queue.empty()){

		current = jedi_queue.top();

		//cout << current.first << " " << current.second << endl;
		
		if(current.first > occupied_pos && current.second < n_segments) {
			occupied_pos = current.second;
			res++;
		}

		jedi_queue.pop();

	}

	cout << res << endl;
}


int main(){
	std::ios_base::sync_with_stdio(false);
	
	int t; cin >> t;

	for(int i=0; i<t; i++){
		testcase();
	}

	return 0;
}