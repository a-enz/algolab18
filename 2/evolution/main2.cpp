// ALGOLAB BGL Tutorial 1
// Tutorial example problem

// Compile and run with one of the following:
// g++ -std=c++11 -O2 bgl-tutorial_problem.cpp -o bgl-tutorial_problem; ./bgl-tutorial_problem < bgl-tutorial_problem.in
// g++ -std=c++11 -O2 -I path/to/boost_1_58_0 bgl-tutorial_problem.cpp -o bgl-tutorial_problem; ./bgl-tutorial_problem < bgl-tutorial_problem.in

// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <unordered_map>

// Namespaces
using namespace std;

unordered_map<string, int> indexmap;
vector<string> species_name;
vector<int> species_age;
vector< vector<int> > tree;

void dfs(int v, vector<int>& path, 
                vector< vector< pair<int, int> > > query, 
                vector<int>& answers) {
    //update path
    path.push_back(v);
    
    //answer queries
    for (int i = 0; i < query[v].size(); i += 1)
    {
        int age = query[v][i].first;
        int q_idx = query[v][i].second;
        
        auto species = upper_bound(path.begin(), path.end(), age, 
            [&](const int& val, const int& element) -> bool {
                return !(val < species_age[element]);
            });
            
        assert(answers[q_idx] == -1);
        answers[q_idx] = *species;
    }
    
    
    //go deeper into the tree
    for(int u : tree[v]) {
        dfs(u, path, query, answers);
    }
    
    //update path
    path.pop_back();
}

// Functions
// ========= 
void testcases() {
    int n_species, q_queries;
    cin >> n_species >> q_queries;
    
    indexmap.clear();
    
    species_name = vector<string>(n_species);
    species_age= vector<int>(n_species);
    for (int i = 0; i < n_species; i += 1)
    {
        string s; int a;
        cin >> s >> a;
        species_name[i] = s;
        species_age[i] = a;
        indexmap[s] = i;
    }
    
    //find out index of luca (oldest age)
    int luca_idx = distance(species_age.begin(),
                    max_element(species_age.begin(), species_age.end()));
    
    
    //read relation and build the tree with connections from parent to all child nodes
    tree = vector< vector<int> >(n_species);
    for (int i = 0; i < n_species-1; i += 1)
    {
        string parent, child;
        cin >> child >> parent;
        tree[indexmap[parent]].push_back(indexmap[child]);
    }
    
    //read queries and store queries for each starting node
    vector< vector< pair<int, int> > > query(n_species); //store pair of (age, index)
    for (int i = 0; i < q_queries; i += 1)
    {
        string s; int b;
        cin >> s >> b;
        query[indexmap[s]].push_back(make_pair(b, i));
    }
    
    
    //answer queries
    vector<int> answers(q_queries,-1);
    vector<int> path;
    
    dfs(luca_idx, path, query, answers);
    
    for (int i = 0; i < q_queries; i += 1)
    {
        assert(answers[i] != -1);
        cout << species_name[answers[i]] << " ";
    }
    cout << endl;
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


