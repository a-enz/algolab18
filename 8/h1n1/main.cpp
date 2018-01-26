#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <map>
#include <queue>
#include <climits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;
typedef Triangulation::Finite_edges_iterator  Edge_iterator;
typedef Triangulation::Face_handle  Face_handle;
typedef K::Point_2  P;
typedef Triangulation::Vertex_handle    Vertex_handle;
typedef Triangulation::Face_circulator  Face_circ;

const double DMAX =  numeric_limits<double>::max();


bool finds_way_out(const Triangulation &t, const Face_handle &f, 
                const double d, unordered_set<Face_handle> &visited) {
    
    // check if we reached infinite face 
    if(t.is_infinite(f) || f == NULL) 
        return true;

    visited.insert(f);
    // iterate over edges of face and check distance condition
    // if distance condition holds: 
    // open new recursion for every neighboring face no yet visited


    for (int i = 0; i < 3; ++i)
    {
        P p1 = f->vertex(i)->point();
        P p2 = f->vertex((i+1) % 3)->point();
        K::FT dist = CGAL::squared_distance(p1, p2);

        Face_handle nf = f->neighbor((i+2) % 3);

        //check distance
        //check nf not yet visited
        if(4*d <= dist && 
            visited.find(nf) == visited.end()) {

            if(finds_way_out(t, nf, d, visited)) {
                return true;
            }
        }
    }

    //only reached if no recursion returns 'true'
    return false;
}

void bfs_escape_rad(const Triangulation &t, map<Face_handle, K::FT>& escape_map) {
    //start from infinite faces
    Face_circ f = t.incident_faces(t.infinite_vertex());
    queue<Face_handle> Q;
    
    //push all infinite faces  
    do {
        Q.push(f);
    } while (++f != t.incident_faces(t.infinite_vertex()));
   
    while(not Q.empty()) {
        const Face_handle current = Q.front();
        Q.pop();
        
        if(t.is_infinite(fh) && escape_map.find(fh) == escape_map.end()) {
            escape_map[fh] = DMAX;
            //continue with the finite neighbors 
        }
        else {
        
        }
            
            
            
        
    }
}


bool can_escape(const Triangulation &t, const P& start, const double d) {
    // first check how far the start is from the nearest infected
    Vertex_handle nearest = t.nearest_vertex(start);
    double dist_nearest = CGAL::squared_distance(nearest->point(), start);


    if(dist_nearest < d) {
        return false;
    }
    else {
        // check if there is an escape route
        // do a bfs or dfs over all edges and try to get to
        // the 'outside' by reaching infinite face

        // locate starting face using a hint
        Face_handle f = t.locate(start, t.incident_faces(nearest)); 

        //keep track of visited faces
        unordered_set<Face_handle> visited;

        //do dfs
        return finds_way_out(t, f, d, visited);
    }
}

void testcase(int n_infected) {

    //read infected locations
    vector<P> pts(n_infected);
    for (int i = 0; i < n_infected; ++i)
    {
        cin >> pts[i]; 
    }

    // read query locations and radii
    int m_queries;
    cin >> m_queries;
    vector<P> query_loc(m_queries);
    vector<double> query_radius(m_queries);

    for (int i = 0; i < m_queries; ++i)
    {
        cin >> query_loc[i];
        cin >> query_radius[i];
    }

    // Triangulation of infected:
    Triangulation t;
    t.insert(pts.begin(), pts.end());

    // process queries:
    for(int i=0; i<m_queries; i++)
    {
        if(can_escape(t, query_loc[i], query_radius[i])) {
            cout << "y";
        }
        else {
            cout << "n";
        }
    }

    cout << endl;
}

int main(void) {

    //disable for performance
    ios_base::sync_with_stdio(false);
    //read total number of test cases
    int n;
    cin >> n;

    //do for each testcase
    while(n != 0){
        testcase(n);
        cin >> n;
    }

    return 0;
}
