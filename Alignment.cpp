/***************************************************************************************
 * PROBLEM SPECIFICATION
 ***************************************************************************************
 * GOAL: to determine the edit distance between and an optimal alignment for two words.
 *
 * INPUT:
 * The input to our program is two words to be aligned, given as an ASCII character
 * stream on stdin, consisting of two lines separated by a newline. The two lines
 * contain the words to be aligned, each word on a line by itself, and each consisting
 * of a sequence of characters from the alphabet a-z containing no spaces or other
 * whitespace characters.
 * 
 * For example, an input could be
 *      abracadabra
 *      candelabra
 *
 * OUTPUT:
 * The output of this program is the edit distance and an optimal alignment, represented
 * as an ASCII character stream on stdout consisting of three lines, each terminated with
 * a newline. The first line is a single nonnegative integer representing the edit
 * distance. The second and third should contain the two words, one on each line, with
 * spaces inserted to show an optimal alignment.
 * 
 * For example, an output could be
 *      7
 *       abracadabra
 *      candela  bra
 *
 * ASSUMPTIONS:
 * - Any input will conform exactly to the format described above.
 * - We will be given large instances of the problem to handle.
 * - Any optimal alignment with the correct edit distance will be accepted.
 *
 ***************************************************************************************/

#include <iostream>
#include <vector>

using namespace std;

// Helper function that returns the minimum of three integer values
int min3(int n1, int n2, int n3) {
    return min(min(n1, n2), n3);
}

// Recursive function that returns the minimum cost of an alignment between
// x1x2...xi and y1y2...yj, storing optimal values at A[i, j]
int opt(vector<vector<int>>* A, string* x, string* y, int i, int j) {
    // If A[j, i] has already been calculated, then return that value
    if ((*A)[i][j] >= 0) return (*A)[i][j];
    // Otherwise, use recurrence to compute optimal alignment value
    // Get value of mismatch penalty
    int mismatch = ((*x)[i-1] == (*y)[j-1]) ? 0 : 1;
    // Get value of recurrence relation OPT(i, j)
    int rec = min3(mismatch + opt(A, x, y, i - 1, j - 1), 1 + opt(A, x, y, i - 1, j),
        1 + opt(A, x, y, i, j - 1));

    // Store and return optimal alignment value
    (*A)[i][j] = rec;
    return rec;
}

// Function that traces back through the array A and returns the optimal alignment itself,
// as well as the first string spaced out to correctly align with the optimal alignment
vector<string> getOptAlignment(vector<vector<int>>* A, string* x, string* y, int m, int n) {
    // Initialize variables to track location in array A
    int i = m;
    int j = n;
    // Initialize variables for values to the top, left, and top left of A[i, j]
    int valTop, valLeft, valTopLeft;
    // Initialize variable for minimum surrounding value of A[i, j]
    int minVal;
    // Initialize variable for correctly-spaced original string result
    string ogStr;
    // Initialize variable for optimal alignment result
    string optStr;

    // Trace back through array until we reach the end of either string
    while (i > 0 && j > 0 ) {
        // Get surrounding values
        valTop = (*A)[i-1][j];
        valLeft = (*A)[i][j-1];
        valTopLeft = (*A)[i-1][j-1];
        // Get minimum surrounding value
        minVal = min3(valTop, valLeft, valTopLeft);

        // If (i, j) is an optimal alignment
        if (minVal == valTopLeft) {
            ogStr = (*x)[i - 1] + ogStr;
            optStr = (*y)[j - 1] + optStr;
            i--;
            j--;
        }
        // Else check left value
        else if (minVal == valLeft) {
            // Add space to original string
            ogStr = ' ' + ogStr;
            optStr = (*y)[j - 1] + optStr;
            j--;
        }
        // Else go up
        else {
            // Add space to optimal alignment
            ogStr = (*x)[i - 1] + ogStr;
            optStr = ' ' + optStr;
            i--;
        }
    }

    // If anything is left in either string, add it now
    string spaces;
    if (i > 0) {
        spaces.assign(i, ' ');
        ogStr = (*x).substr(0, i) + ogStr;
        optStr = spaces + optStr;
    }
    if (j > 0) {
        spaces.assign(j, ' ');
        ogStr = spaces + ogStr;
        optStr = (*y).substr(0, j) + optStr;
    }

    // Construct vector for result and return
    vector<string> result = { ogStr, optStr };
    return result;

}

int main()
{
    // As given by the problem, we will be using a gap penalty of 1 and mismatch
    // penalties of 1 for all character pairs, except when those characters are
    // the same, in which case the mismatch penalty is 0.

    // Initialize strings
    string x, y;
    // Read in inputs
    cin >> x;
    cin >> y;

    // Get lengths of strings
    int m = x.length();
    int n = y.length();

    // Construct array A for the optimal values
    vector<vector<int>> A;
    // Initialize A[i, 0] for each i
    vector<int> xvec;
    xvec.assign(n+1, -1);
    for (int i = 0; i <= m; i++) {
        xvec[0] = i;
        A.push_back(xvec);
    }
    // Initialize A[0, j] for each j
    vector<int> yvec;
    for (int j = 0; j <= n; j++) {
        yvec.push_back(j);
    }
    A[0] = yvec;

    // Get pointer to A
    vector<vector<int>>* Aptr = &A;

    // Get optimal alignment value
    int optAlignVal = opt(Aptr, &x, &y, m, n);
    // Get optimal alignment itself
    vector<string> optAlignment = getOptAlignment(Aptr, &x, &y, m, n);

    // Print output in correct format
    cout << optAlignVal << endl
        << optAlignment[0] << endl
        << optAlignment[1] << endl;
}