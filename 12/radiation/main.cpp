#undef NDEBUG
#include <iostream>
#include <cassert>

#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// Namespaces
using namespace std;

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef vector< vector< vector<ET> > > LT;

LT compute_lookup(const vector< vector<ET> >& points) {
    //for each point i:
    //  compute each possible product x^a * y^b * z^c
    //  thus the four tuple (i, a, b, c) maps to the product
    //  (i, a, b, c) -> x_i^a * y_i^b * z_i^c

    LT table(points.size());
    for (size_t i = 0; i < table.size(); ++i)
    {   
        table[i] = vector< vector<ET> >(31, vector<ET>(3));
        table[i][0][0] = ET(1);
        table[i][0][1] = ET(1);
        table[i][0][2] = ET(1);

        //assert(table[i].size() == 31);
        for (size_t exp = 1; exp < table[i].size(); ++exp)
        {
            table[i][exp][0] = table[i][exp-1][0] * points[i][0];
            table[i][exp][1] = table[i][exp-1][1] * points[i][1];
            table[i][exp][2] = table[i][exp-1][2] * points[i][2];
        }
    }  

    return table;
}



ET mult_and_exp(const vector< vector<ET> >& table, int a, int b, int c) {
    //lookup table for some specific point (x, y, z)
    //x^a * y^b * z^c
    return table[a][0] * table[b][1] * table[c][2];
}

bool too_small(int degree, const LT& cells_table,
                                const LT& tumors_table) {

    Program lp (CGAL::SMALLER, false, 0, false, 0);
    int var_counter = 0;

    for (int a=0; a <= degree; ++a)
    {
        for (int b=0; b <= degree-a; ++b)
        {
            for (int c=0; c <= degree-a-b; ++c)
            {   
                for (size_t i = 0; i < tumors_table.size(); ++i)
                {   
                    //x^a * y^b * z^c
                    lp.set_a(var_counter, i, 
                        mult_and_exp(tumors_table[i], a, b, c));
                }

                //set cell constraints
                for (size_t i = 0; i < cells_table.size(); ++i)
                { 
                    //x^a * y^b * z^c
                    lp.set_a(var_counter, i+tumors_table.size(), 
                        mult_and_exp(cells_table[i], a, b, c));
                }
                
                var_counter++;
            }
        }
    }

    //set tumor constraints boundary and slack variable
    //tumors need to be in areas with positive radiation
    for (size_t i = 0; i < tumors_table.size(); ++i)
    {   
        lp.set_b(i, 1);
        lp.set_r(i, CGAL::LARGER);
    }

    //cells need to be in areas with negative radiation
    for (size_t i = 0; i < cells_table.size(); ++i)
    { 
        int idx = i+tumors_table.size();
        lp.set_b(idx, -1);
        lp.set_r(idx, CGAL::SMALLER);
    }
    
    // solve the program, using ET as the exact type    
    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    Solution s = CGAL::solve_linear_program(lp, ET(), options);
    //assert (s.solves_linear_program(lp));

    //run the LP and check if there exists a solution 
    //and verify that the slack variable is not zero
    return s.is_infeasible();
}

// Functions
// ========= 
void testcases() {
    int h_cells, t_tumors;
    cin >> h_cells >> t_tumors;

    vector< vector<ET> > cells(h_cells, vector<ET>(3)); //x, y, z
    vector< vector<ET> > tumors(t_tumors, vector<ET>(3));

    for (int i = 0; i < h_cells; ++i)
    {   
        int x, y, z;
        cin >> x >> y >> z;
        cells[i] = {ET(x), ET(y), ET(z)};
    }

    for (int i = 0; i < t_tumors; ++i)
    {        
        int x, y, z;
        cin >> x >> y >> z;
        tumors[i] = {ET(x), ET(y), ET(z)};
    }

    //check special case
    if(h_cells == 0 || t_tumors == 0) {
        cout << "0\n";
        return;
    }

    //construct exponent table: for each 
    //exponent 'exp' and possible point (x, y, z) compute x^exp, y^exp, z^exp
    LT cells_table = compute_lookup(cells);
    LT tumors_table = compute_lookup(tumors);

    //do exponential and binary search 
    int lmin = 1, lmax = 1;
    while(lmax <=30 && too_small(lmax, cells_table, tumors_table)) {
        lmax *= 2;
    }
    
    //check boundary
    if(high > 30 && too_small(30)) {
        cout << "Impossible!\n";
        return;
    }
    
    //clip upper bound
    high = min(high, 30);
    
    while(lmin < lmax) {
        int mid = lmin + (lmax - lmin) / 2;
        assert(mid <= 30);
        assert(mid >= 1);
        if(too_small(mid, cells_table, tumors_table))
            lmin = mid+1;
        else
            lmax = mid;
    }
    
    cout << lmax << endl;

}

// Main function looping over the testcases
int main() {
    ios_base::sync_with_stdio(false);
    int T;  cin >> T;   // First input line: Number of testcases.
    while(T--)  testcases();
    return 0;
}

