// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Bev {
    int cost;
    int vol;
    int costprio;
};

void testcases() {
    size_t n_bev, k_ppl;
    cin >> n_bev >> k_ppl;
    
    vector<Bev> beverage(n_bev);
    int min_costprio = INT_MAX;
    
    for (unsigned int i = 0; i < n_bev; i += 1)
    {
        cin >> beverage[i].cost;
        cin >> beverage[i].vol;
           
        int mult = k_ppl / beverage[i].vol;
        if(k_ppl % beverage[i].vol != 0) 
            mult += 1;
        
        beverage[i].costprio = beverage[i].cost * mult;
        
        min_costprio = (min_costprio > beverage[i].costprio) ?
                        beverage[i].costprio : min_costprio;
    }
    
    sort(beverage.begin(), beverage.end(), [](const Bev& a, const Bev& b) -> bool {
        if(a.costprio == b.costprio)
            return a.vol < b.vol;
        else
            return a.costprio < b.costprio;
    });
    
    int total_vol = 0, b = 0;
    while(beverage[b].costprio == min_costprio && total_vol < k_ppl) {
        total_vol += beverage[b].vol;
        b++;
    }
    
    cout << beverage[0].costprio << " " << b << endl;
    
}

// Main function looping over the testcases
int main() {
	ios_base::sync_with_stdio(false);
	int T;	cin >> T;	// First input line: Number of testcases.
	while(T--)	testcases();
	return 0;
}


