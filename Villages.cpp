/***************************************************************************************
 * PROBLEM SPECIFICATION
 ***************************************************************************************
 * GOAL: to determine the minimum number of new roads needed to ensure that every
 *		 village in the region is accessible to every other village by some sequence
 *		 of roads.
 *
 * INPUT:
 * The input to our program is a representation of the current road system for the
 * region, given as an ASCII character stream on stdin, consisting of several lines
 * separated by a newline. The first line will contain two non-negative integers [n]
 * and [m] separated by a space, denoting the number of villages and the number of
 * currently existing roads, respectively. The names of the villages are numbers from
 * 0 to n-1, inclusive. Each of the next [m] lines will contain two integers [u] and
 * [v] separated by a space, denoting a road between villages [u] and [v]. The roads
 * are assumed to be bidirectional, meaning that traffic may travel in either direction.
 *
 * OUTPUT:
 * The output of this program is the minimum number of new roads needed to ensure every
 * village is reachable from every other village, represented as a single integer
 * terminated by a newline.
 *
 * ASSUMPTIONS:
 * - Any input will conform exactly to the format described above.
 * - We will be given large instances of the problem to handle.
 *
 ***************************************************************************************/

#include <iostream>
#include <list>
#include <vector>
#include <set>

using namespace std;

/***************************************************************************************
 * COUNTY CLASS DECLARATION
 ***************************************************************************************/
 // Represents a subgroup of villages that are fully connected by roads, in that
 // every village in the County can access every other village by some sequence of roads
class County {
	list<int> villages; // List of villages that compose the county
public:
	County(int u, int v);
	list<int>* getVillages() { return &villages; } // Getter for villages in county
	void addVillage(int v);
	void mergeCounties(list<int>* vptr);
};

// Constructor for county with just villages u, v
County::County(int u, int v) {
	villages.push_back(u);
	villages.push_back(v);
}

// Adds the village [v] to the county
void County::addVillage(int v) {
	villages.push_back(v);
}

// Merges the villages for the current county and the villages at [vptr]
void County::mergeCounties(list<int>* vptr) {
	villages.merge(*vptr);
}

/***************************************************************************************
 * REGION CLASS DECLARATION
 ***************************************************************************************/
 // Represents all villages in the region, organized by county
class Region {
	vector<County*> directory; // Dictionary: key = village, value = county of village
	int numCounties = 0; // Total number of counties in the region
	int villagesRemaining; // Number of villages not in a county
public:
	Region(int n);
	int getNumCounties();
	void addRoad(int u, int v);
	void cleanup();
};

// Constructor for a new region with [n] villages
Region::Region(int n) {
	villagesRemaining = n;
	// Initialize directory with all villages unassigned to a county
	directory.assign(n, NULL);
}

// Returns the true total number of counties, including single villages
int Region::getNumCounties() {
	return (numCounties + villagesRemaining);
}

// Updates the counties of the region knowing there's a road between villages u, v
void Region::addRoad(int u, int v) {
	// Get counties of both villages
	County* uCounty = directory[u];
	County* vCounty = directory[v];

	// If both villages have not been assigned counties yet
	if (uCounty == NULL && vCounty == NULL) {
		// Create new county containing villages u, v
		County* newCounty = new County(u, v);
		// Update directory with county for villages u, v
		directory[u] = newCounty;
		directory[v] = newCounty;
		// Update numCounties and villagesRemaining
		numCounties++;
		villagesRemaining = villagesRemaining - 2;
	}
	// Else if just village [u] has not been assigned a county yet
	else if (uCounty == NULL) {
		vCounty->addVillage(u); // Add u to vCounty
		directory[u] = vCounty; // Update directory
		villagesRemaining--;
	}
	// Else if just village [v] has not been assigned a county yet
	else if (vCounty == NULL) {
		uCounty->addVillage(v); // Add v to uCounty
		directory[v] = uCounty; // Update directory
		villagesRemaining--;
	}
	// Else if both villages are in different counties
	else if (uCounty != vCounty) {
		// Get list of villages in vCounty
		list<int>* vVillages = vCounty->getVillages();
		// Update directory for all villages formerly in vCounty
		for (auto v : *vVillages) {
			directory[v] = uCounty;
		}
		// Combine counties by having uCounty absorb vCounty
		uCounty->mergeCounties(vVillages);
		// Decrement total number of counties
		numCounties--;
		// Deallocate vCounty
		delete(vCounty);
	}
}

// Deallocate any remaining pointers in the region. Should only be called at the very
// end of the program, when the region is no longer going to be used
void Region::cleanup() {
	// Convert counties in directory to a set to remove duplicates
	set<County*> sCounties;
	unsigned size = directory.size();
	for (unsigned k = 0; k < size; ++k) { sCounties.insert(directory[k]); }

	// Iterate through set, deallocating all remaining pointers
	for (auto s : sCounties) {
		delete(s);
	}
}

/***************************************************************************************
 * MAIN FUNCTION
 ***************************************************************************************/
int main() {
	// Get initial input (n, m)
	// n = number of villages, m = number of currently existing roads
	int n, m;
	scanf("%d%d", &n, &m);

	// Initialize region
	Region reg = Region(n);

	// Initialize subsequent inputs (u, v)
	// Represents a road between village u and village v
	int u, v;

	// Iterate through subsequent inputs and add corresponding roads to region
	for (int k = 0; k < m; k++) {
		scanf("%d%d", &u, &v);
		reg.addRoad(u, v);
	}

	// Minimum number of roads needed is given by the number of roads needed to connect
	// all counties, which is given by getNumCounties() - 1, so we will print that result
	printf("%d\n", reg.getNumCounties() - 1);

	// Deallocate any remaining pointers
	reg.cleanup();
}