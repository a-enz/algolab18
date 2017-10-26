#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

void run_test() {
	int n;
	cin >> n;

	vector< vector<int> > partial_sums(n, vector<int>(n));

	int odd=0, even=0;

	for(int row=0; row < n; row++) {
		for(int col=0; col < n; col++) {

			int val;
			cin >> val;

			//cout << "(" << row << ", " << col << "):" << val << endl;

			if(col==0 && row==0) partial_sums[row][col] = val;
			else if(col==0) 
			{
				partial_sums[row][col] = partial_sums[row-1][col] + val;
			}
			else if(row==0)
			{
				partial_sums[row][col] = partial_sums[row][col-1] + val;
			}
			else 
			{
				partial_sums[row][col] = partial_sums[row-1][col] + 
										partial_sums[row][col-1] -
										partial_sums[row-1][col-1] + 
										val;
			}

			if(partial_sums[row][col] % 2 == 0) even++;
			else odd++;

			//cout << partial_sums[row][col] << " ";
		}
	}

	even=0; odd=0;

	for(int i1=0; i1<n; i1++){
		for(int j1=0; j1<n; j1++) {
			for(int i2=i1; i2<n; i2++) {
				for(int j2=j1; j2<n; j2++) {
					int sum = 0;

					if(i1==0 && j1==0) {
						sum = partial_sums[i2][j2];
					}
					else if(i1==0) 
					{
						sum = partial_sums[i2][j2] - partial_sums[i2][j1-1];
					}
					else if(j1==0)
					{
						sum = partial_sums[i2][j2] - partial_sums[i1-1][j2];
					}
					else 
					{
						sum = partial_sums[i2][j2] -
							partial_sums[i1-1][j2] -
							partial_sums[i2][j1-1] +
							partial_sums[i1-1][j1-1];
					}

					if(sum % 2 == 0) even++;
				}
			}
		}
	}

	cout << even << endl;
	int result = even; //+even choose 4 + even choose 2 * odd choose 2 + odd choose 4;
}

int main(void) {

	//disable for performance
	ios::sync_with_stdio(false);
	//read total number of test cases
	int n_total_testcases;
	cin >> n_total_testcases;

	//do for each testcase
	for(int n_test = 0; n_test < n_total_testcases; n_test++) {

		run_test();
	}

	return 0;
}