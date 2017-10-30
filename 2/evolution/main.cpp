#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;

void testcase() {
	int n_species, n_queries;
	cin >> n_species >> n_queries;

	//store names (essentially a map: idx-> name)
	vector<string> species_name(n_species);
	map<string, int> name_to_idx;
	//species age
	vector<int> species_age(n_species);
	//store the tree by species ancestor
	//luca (root) points to itself
	vector<int> ancestor(n_species);

	//store queries as pair (name, age)
	vector< pair<string, int> > queries(n_queries);

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
	ancestor[root_idx] = root_idx;
	for(int i=0; i<n_species-1; i++) {

		string species, anc_species;
		cin >> species >> anc_species;

		ancestor[name_to_idx[species]] = name_to_idx[anc_species];

/*		cout << "species " << species << ":" << name_to_idx[species] << " has ancestor "
			<< anc_species << ":" << name_to_idx[anc_species] << endl;
*/	}


	//read queries
	for(int i=0; i<n_queries; i++) {
		string species;
		int age_limit;
		cin >> species >> age_limit;

		queries[i] = make_pair(species, age_limit);
	}


	//naive solution
	for(vector< pair<string, int> >::iterator q_it = queries.begin();
		q_it != queries.end();
		q_it++) {

		int child_species = name_to_idx[q_it->first];
		int age_limit = q_it->second;

/*		cout << "age limit: " << age_limit << endl;
*/
		if(child_species == ancestor[child_species]) { //points to itself, in other words luca
			cout << q_it->first << " "; //luca /root
			continue;
		}

		int ancestor_species = ancestor[child_species];

		while(child_species != ancestor_species &&
			species_age[ancestor_species] <= age_limit) {
			child_species = ancestor_species;
			ancestor_species = ancestor[child_species];
		}

		cout << species_name[child_species] << " ";
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