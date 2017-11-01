#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <numeric>
#include <string>
#include <algorithm>

using namespace std;

int binary_search(const vector<int>& path, 
					const vector<int>& species_age,
					const int age_limit) {
	//search on path of (species) nodes for species age
	//that is the largest age <= age_limit
	int higher=0, lower=path.size()-1;

	while(higher != lower) {
		int p = (higher+lower)/2;
		if(species_age[path[p]] > age_limit)
			higher = p+1;
		else
			lower = p;
	}

	return path[lower];
}


void dfs(const int u, 
	const vector< vector<int> >& tree, 
	vector<int>& path, 
	const vector< vector< pair<int, int> > >& queries, 
	vector<int>& results, 
	const vector<int>& species_age) {

	//process queries for u
	for(int i=0; i<queries[u].size(); i++) {
		//do binary search on path for age limit
		int age_limit = queries[u][i].first;
		int query_idx = queries[u][i].second;

		results[query_idx] = binary_search(path, species_age, age_limit);
	}

	//continue dfs
	for(int i=0; i<tree[u].size(); i++) {
		int child = tree[u][i];
		path.push_back(child);
		dfs(child, tree, path, 
			queries, results, species_age);
	}

	path.pop_back();
}

void testcase() {
	int n_species, n_queries;
	cin >> n_species >> n_queries;

	//store names (essentially a map: idx-> name)
	vector<string> species_name(n_species);
	unordered_map<string, int> name_to_idx;
	//species age
	vector<int> species_age(n_species);
	//build tree by storing children of parent
	//in adjacency list
	vector< vector<int> > tree(n_species, vector<int>());

	//for each species store a vector consisting of
	//a pair (b, i) where b: age limit, i: query index
	vector< vector< pair<int, int> > > queries(n_species);

	//read age and create species idx map
	int root_idx, max_age=0;
	for(int i=0; i<n_species; i++) {
		string name;
		int age;
		cin >> name >> age;

		species_name[i] = name;
		species_age[i] = age;
		name_to_idx[name] = i;

		if(age > max_age) {
			max_age = age;
			root_idx = i;
		}
	}

	//read tree
	for(int i=0; i<n_species-1; i++) {
		string child, parent;
		cin >> child >> parent;

		tree[name_to_idx[parent]].push_back(name_to_idx[child]);
	}


	//read queries
	for(int i=0; i<n_queries; i++) {
		string species;
		int age_limit;
		cin >> species >> age_limit;

		queries[name_to_idx[species]].push_back(
			make_pair(age_limit, i)
		);
	}


	//do dfs traversal and binary search on the paths
	//if we reach a species we have a query for
	//store query result in a vector
	vector<int> results(n_queries);
	//initialize path
	vector<int> path;
	path.push_back(root_idx);
	dfs(root_idx, tree, path, queries, results, species_age);

	//print result vector in the correct order
	for(int i=0; i<n_queries; i++) {
		cout << species_name[results[i]] << " ";
	}

	cout << endl;

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