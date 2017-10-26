#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits.h>

using namespace std;

struct boat {
	int length;
	int position;
};

bool compareBoats(const boat&left, const boat& right) {
	return left.position < right.position;
}

int main(void) {

	//disable for performance
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);
	//read total number of test cases
	int n_total_testcases;
	cin >> n_total_testcases;

	//do for each testcase
	for(int n_test = 0; n_test < n_total_testcases; n_test++) {

		int n_boats;
		cin >> n_boats;
		vector<boat> boats(n_boats);

		for(int i = 0; i < n_boats; i++) {
			cin >> boats[i].length;
			cin >> boats[i].position;
		}

		sort(boats.begin(), boats.end(),compareBoats);

		//first boat always fits
		int length_of_boats = boats[0].position;
		int current_boat = 0;		
		int boat_counter = 1;
		int test_counter = 0;

		//search until we have gone further then the last boat ring position
		while(length_of_boats <= boats[n_boats - 1].position && current_boat < n_boats) 
		{
			//search for shortest boat (and maybe buffer of water)
			//starting from length_of_boats
			int new_boat = current_boat+1;
			int new_length = INT_MAX;

			while(boats[new_boat].position < new_length &&
				new_boat < n_boats) {
				//if we would use current boat, how much length would it add
				//to the total length of the line of boats

				if(boats[new_boat].position < length_of_boats) {
					new_boat++;
					current_boat = new_boat;
					continue;
				}

				int dist_next_ring = boats[new_boat].position - length_of_boats;
				int added_length = (boats[new_boat].length > dist_next_ring) ?
									boats[new_boat].length : dist_next_ring;

				int possible_new_length = length_of_boats + added_length;

				if(possible_new_length <= new_length) {
					new_length = possible_new_length;
					current_boat = new_boat;
				}
				new_boat++;
			}

			if(new_length != INT_MAX) {
				boat_counter++;
				length_of_boats = new_length;
			}
		}

		//write output values for testcase
		cout << boat_counter << '\n';
	}

	return 0;
}