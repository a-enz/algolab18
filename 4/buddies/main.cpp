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

const Vertex NULL_VERTEX = boost::graph_traits<Graph>::null_vertex();


void testcase() {

	int V, c, f;
	cin >> V >> c >> f;

	Graph G(V);
	vector< vector<string> > chara(V, vector<string>(c));

	//read characteristics
	for(int i=0; i<V; i++) {
		for(int j=0; j<c; j++) {
			cin >> chara[i][j];
		}

		sort(chara[i].begin(), chara[i].end());
	}

	//for each possible pair of student intersect the characteristics
	//set and check if it is larger than f, if yes add edge to G
	for(int i=0; i<V; i++) {
		for(int j=i+1; j<V; j++) {

			int p1=0, p2=0;
			int common_count = 0;
			while(p1 < chara[i].size() && p2 < chara[j].size() && common_count <= f) {
				if(chara[i][p1] == chara[j][p2]) {
					common_count++;
					p1++;
					p2++;
				}
				else if(chara[i][p1] < chara[j][p2]) {
					p1++;
				}
				else {
					p2++;
				}
			}

			//add edge
			if(common_count > f){
				add_edge(i, j, G);
			}
		}
	}

	//now compute maximum matching
	vector<Vertex> matemap(V);
	edmonds_maximum_cardinality_matching(G, 
		make_iterator_property_map(matemap.begin(), get(vertex_index, G)));

	int matchingsize = matching_size(G, make_iterator_property_map(matemap.begin(), get(vertex_index, G)));
	//compute maximum matching by hand:

/*	bool success = true;
	for (int i = 0; i < V; ++i)
	{
		if(matemap[i] == NULL_VERTEX) {
			success = false;
			break;
		}
	}
*/

	//check that the matching size is half of the student count 
	if(matchingsize == V/2) cout << "not optimal\n";
	else cout << "optimal\n";
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