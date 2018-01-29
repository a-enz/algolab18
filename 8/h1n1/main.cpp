#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <climits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

using namespace std;


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;
typedef Triangulation::Finite_edges_iterator  Edge_iterator;
typedef Triangulation::Face_handle  Face_handle;
typedef K::Point_2  P;
typedef Triangulation::Vertex_handle    Vertex_handle;
typedef Triangulation::Face_circulator  Face_circ;

const double DMAX =  numeric_limits<double>::max();

void bfs_escape_rad(const Triangulation &t) {
    //initialize info with small values for each finite face
    for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f)
        f->info() = 0;

    //start from infinite faces
    Face_circ f = t.incident_faces(t.infinite_vertex());
    queue<Face_handle> Q;
    
    //push all infinite faces and initialize escape radii to large value
    do {
        Q.push(f);
        f->info() = DMAX;
    } while (++f != t.incident_faces(t.infinite_vertex()));
   
    while(not Q.empty()) {
        const Face_handle pf = Q.front();
        Q.pop();
        
        //go over all neighboring faces that are not infinite
        //check the border edge and update maximal escape radius   
        for (int i = 0; i < 3; ++i)
        {
            Face_handle cf = pf->neighbor(i);
            if(not t.is_infinite(cf)) {
                //check the maximal radius that allows to enter the child face
                Vertex_handle v1 = pf->vertex((i+1) % 3);
                Vertex_handle v2 = pf->vertex((i+2) % 3);

                //propagate the maximum radius of parent
                //but shrink that distance if the edge we're crossing has smaller
                //length
                K::FT dist = CGAL::squared_distance(v1->point(), v2->point());
                dist = min(dist, pf->info());

                //update child info if we found a larger escape radius
                if(dist > cf->info()) {
                    cf->info() = dist;
                    Q.push(cf);
                }
            }
        }
    }
}


bool can_escape(const Triangulation& t, const P& start, const double d) {
    // first check how far the start is from the nearest infected
    Vertex_handle nearest = t.nearest_vertex(start);
    double dist_nearest = CGAL::squared_distance(nearest->point(), start);

    if(dist_nearest < d) {
        return false;
    }
    else {
        Face_handle f = t.locate(start);
        //safety radius is sqrt(d)
        //thus (2*sqrt(d))^2 <= squared_distance allows escape
        return 4 * d <= f->info();
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

    //do a bfs and compute the maximum allowed escape radius for each face
    bfs_escape_rad(t);

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
