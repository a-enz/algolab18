// Includes
// ========
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <set>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

// Namespaces
using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef K::Point_2 P;
typedef K::Circle_2 Circle;

//Idea: 
//Triangulation of all the lamp locations. Since all areas lit by lamps have 
//the same radius (lamps facing same direction, at same hight). We can check
//with nearest_vertex for each person if it is too close to a lamp.
//
//If this is efficient enough we can do binary search on the min. amount of lamps
//we need to light such that all persons are covered (if that is possible)
//Knowing the min. amount of lamps we can compute the set of persons remaining
//before switching on one less than the min. amount of lamps


int m_participants, n_lamps;
vector<P> participant_loc;
vector<long> participant_rad;
long lamp_height;
vector<P> lamp_loc;
// Functions
// ========= 


vector<bool> compute_hits(int p) {
                
    //Triangulation of the lamps
    Triangulation t;
    t.insert(lamp_loc.begin(), lamp_loc.begin() + p);
    

 
    //check for each person if it is hit by a lamp:
    vector<bool> is_hit(m_participants, false);
    for (unsigned int i = 0; i < m_participants; i += 1)
    {
        P nearest = t.nearest_vertex(participant_loc[i])->point();
        K::FT sq_dist = CGAL::squared_distance(nearest, participant_loc[i]);

        K::FT d = lamp_height * lamp_height;
        d = d + participant_rad[i] * participant_rad[i];
        d = d + 2 * lamp_height * participant_rad[i];
        //DIST^2 < participant_rad[i]^2 + 
        //          lamp_height^2 + 2 * lamp_height * participant_rad[i]
        if(sq_dist < d) {
            is_hit[i] = true;
        }
    }
    
    return is_hit;
}



void testcases() {
    cin >> m_participants >> n_lamps;
    
    participant_loc = vector<P>(m_participants);
    participant_rad = vector<long>(m_participants);   
    
    for (unsigned int i = 0; i < m_participants; i += 1)
    {
        int x, y;
        long r;
        cin >> x >> y >> r;
        
        participant_loc[i] = P(x, y);
        participant_rad[i] = r;
    }
    
    //the lamp hight is the same as the radius of the light on the stage
    cin >> lamp_height;
    
    lamp_loc = vector<P>(n_lamps);
    for (unsigned int i = 0; i < n_lamps; i += 1)
    {
        int x, y;
        cin >> x >> y;
        
        lamp_loc[i] = P(x, y);
    }
    
    //Do binary search
    int lmin = 0, lmax = n_lamps;
    
    while(lmin < lmax) {
        int p = lmin + (lmax - lmin) / 2;
        
        
        //compute amount of hit persons
        vector<bool> is_hit = compute_hits(p);
        
        int hitcount = 0;
        for (unsigned int i = 0; i < m_participants; i += 1)
        {
            if(is_hit[i]) {
                hitcount++;
            }
        }
        
        if(hitcount < m_participants) {
            lmin = p + 1;
        }
        else
            lmax = p;
    }
    //cout << "lmin " << lmin << " lmax " << lmax << endl;
    //lmin = (lmin == 0) ? lmin : lmin - 1;
    
    vector<bool> is_hit = compute_hits(lmin);
    
    for (unsigned int i = 0; i < m_participants; i += 1)
    {
        if(not is_hit[i]) cout << i << " ";
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


