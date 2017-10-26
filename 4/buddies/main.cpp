// could probably be made faster by using bit operations:
// represent the set of each student by a bit-array (number)
// to compute intersection of sets do bitwise and '&' and count
// number of '1' in result string.
// Problem: need bit array of size 100, which means we cannot just
// use a basic data type like double
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <algorithm>
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
	vector< set<string> > chara(V, set<string>());

	//read characteristics
	for(int i=0; i<V; i++) {
		for(int j=0; j<c; j++) {
			string val;
			cin >> val;

			chara[i].insert(val);
		}
	}

	//for each possible pair of student intersect the characteristics
	//set and check if it is larger than f, if yes add edge to G

	for(int i=0; i<V; i++) {
		for(int j=0; j<V; j++) {
			vector<string> result(c);
			vector<string>::iterator result_it;
			//compute intersection
			result_it = std::set_intersection(chara[i].begin(), chara[i].end(),
									chara[j].begin(), chara[j].end(),
									result.begin()
								);

			result.resize(result_it - result.begin());

			//add edge
			if(result.size() > f){
				add_edge(i, j, G);
			}
		}
	}

	//now compute maximum matching
	vector<Vertex> matemap(V);
	edmonds_maximum_cardinality_matching(G, 
		make_iterator_property_map(matemap.begin(), get(vertex_index, G)));

	const Vertex NULL_VERTEX = graph_traits<Graph>::null_vertex();
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