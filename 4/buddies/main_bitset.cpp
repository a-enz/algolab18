#include <iostream>
#include <vector>
#include <map>
#include <bitset>
#include <cmath>
#include <algorithm>
#include <assert.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS,		
		no_property								
		>											Graph;
typedef graph_traits<Graph>::edge_descriptor		Edge;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;


void testcase() {

	int V, c, f;
	cin >> V >> c >> f;

	Graph G(V);

	//index map of characteristics
	map<string, int> idx_map;
	//create bitset
	vector< bitset<40000> > char_sets(V, bitset<40000>());
	int char_count = 0;

	//read characteristics
	for(int i=0; i<V; i++) {
		for(int j=0; j<c; j++) {
			string val;
			cin >> val;

			if(idx_map.find(val) == idx_map.end()) {
				idx_map[val] = char_count;
				char_count++;
			}

			if(not(idx_map[val] >= 0 && idx_map[val] < 40000)) {
				cout << "shit: " << idx_map[val] << endl;
				exit(1);
			}

			char_sets[i].set(idx_map[val], true);
		}
	}

	//for each possible pair of student intersect the characteristics
	//set and check if it is larger than f, if yes add edge to G

	for(int i=0; i<V; i++) {
		for(int j=0; j<V; j++) {
			bitset<40000> result;
			result = char_sets[i] & char_sets[j];

			//cout << char_sets[i] << " & " << char_sets[j] << endl;

			if(result.count() > f){
				add_edge(i, j, G);
			}
		}
	}

	//now compute maximum matching
	vector<Vertex> matemap(V);
	edmonds_maximum_cardinality_matching(G, 
		make_iterator_property_map(matemap.begin(), get(vertex_index, G)));

	int matchingsize = matching_size(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));

	//check that the matching size is half of the student count 
	if(matchingsize == V/2) cout << "not optimal\n";
	else cout << "optimal\n";
}

int main(void) {

	//disable for performance
	ios::sync_with_stdio(false);
	//read total number of test cases
	int n_total_testcases;
	cin >> n_total_testcases;

	//do for each testcase
	for(int n_test = 0; n_test < n_total_testcases; n_test++) {
		testcase();
	}

	return 0;
}