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
#include <cmath>

// Namespaces
using namespace std;

// Functions
// ========= 
void testcases() {
    size_t N, M;
    cin >> N >> M;
    
    vector<int> room(M);
    for (unsigned int i = 0; i < M; i += 1)
    {
        cin >> room[i];
    }
    
    vector< vector<int> > sr_on(N, vector<int>(M));
    vector< vector<int> > sr_off(N, vector<int>(M));
    for (unsigned int s = 0; s < N; s += 1)
    {
        for (unsigned int r = 0; r < M; r += 1)
        {
            cin >> sr_on[s][r] >> sr_off[s][r];
        }
    }
    
    
    int min_flipped = N+1;
    for (unsigned int i = 0; i < pow(2, N); i += 1)
    {
        vector<int> on(M, 0);
        int flipped = 0;
        for (unsigned int s = 0; s < N; s += 1)
        {
            vector<int>* lights;
            int should_flip = (i>>s) & 1;
            assert(should_flip == 1 || should_flip == 0);
            if(should_flip) {
                flipped++;
                lights = &sr_off[s];
            } else {
                lights = &sr_on[s];
            }
            
            //add the vector to overall lights
            for (unsigned int r = 0; r < M; r += 1)
            {
                on[r] += lights->at(r);
            }
        }
        
        if(on == room && flipped < min_flipped) 
        {
            min_flipped = flipped;
        }
    }
    
    if(min_flipped != N+1) {
        cout << min_flipped << endl;
    }
    else
        cout << "impossible\n";

}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


