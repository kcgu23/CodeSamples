/***************************************************************************************
 * PROBLEM SPECIFICATION
 ***************************************************************************************
 * GOAL: to implement a linear-time depth-first search (DFS) algorithm for directed
 *		 graphs, returning the order of nodes visited in the DFS traversal and the
 *		 types of each edge in the graph (tree, forward, back, cross).
 * 
 * INPUT:
 * The input to our program is a directed graph, given as an ASCII character stream
 * on stdin, consisting of several lines separated by a newline. The first line will
 * contain two non-negative integers [n] and [m] separated by a space, denoting the
 * number of vertices and edges, respectively. The names of the vertices are numbers
 * from 0 to n-1, inclusive. Each of the next [m] lines will contain two integers [u]
 * and [v] separated by a space, denoting a directed edge (u, v). The existence of
 * (u, v) does not imply the existence of an edge (v, u) in the other direction; (v, u)
 * is present if and only if the pair [v], [u] appears in the list.
 * 
 * OUTPUT:
 * The output of this program is the order in which the nodes are first visited in the
 * DFS traversal, as well as the type of each edge: tree, forward, back, or cross.
 * Definitions for these edge types are given in a later comment below. The format is
 * an ASCII character stream on stdout consisting of several lines, each terminated with
 * a newline. The first line consists of [n] integers separated by spaces, representing
 * the names of the nodes listed from left to right in the order that they are first
 * visited. Each integer between 0 and n-1 inclusive should occur exactly once in the
 * list. This is followed by [m] lines, each containing two integers [u] and [v], followed
 * by a single letter [c] separated by spaces. [u] and [v] specify the edge (u, v) and [c]
 * specifies its type, according to the following code:
 *		| Type of Edge | c |
 *		--------------------
 *		|     tree     | t |
 *		|    forward   | f |
 *		|	  back     | b |
 *		|     cross    | c |
 * The output should present the edges in the same order as the input.
 * 
 * ASSUMPTIONS:
 * - Any input will conform exactly to the format described above.
 * - Our code should handle graphs of sizes 0 <= n <= 10^5 and 0 <= m <= 2 * 10^5.
 * - The search should be started from node 0, and we must explore edges (u, v)
 *	 with source node [u] in the order they are given in the input.
 * - If DFS does not reach all nodes, we should restart the DFS from the first
 *   unvisited node, creating a DFS forest of [k] trees if we restart [k] times.
 *
 ***************************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/***************************************************************************************
 * EDGETYPE ENUM DECLARATION
 ***************************************************************************************/
 /* EdgeType Enum (for edge from [u] to [v]). The types of edges are defined as
  * - Undefined: edge has not been traversed or is from a node to itself
  * - Tree: edge that is traversed when visiting [v] for the first time in DFS
  * - Forward: [v] is a descendant of [u], but (u, v) is not a tree edge; that is,
  *			   there is a path of tree edges of length at least two from [u] to [v]
  * - Back: [v] is an ancestor of [u]; that is, there exists a path from [v] to [u]
  *			of tree edges
  * - Cross: all other cases; that is, there is no path of tree edges from [u] to [v]
  *			 nor from [v] to [u]
  */
enum EdgeType { Undefined, Tree, Forward, Back, Cross };

/***************************************************************************************
 * NODE CLASS DECLARATIONS
 ***************************************************************************************/
// Declare class Node
class Node {
	// Vector of immediate children of the current node
	vector<int> children;
	// Vector of types of edge from node to corresponding index child
	vector<EdgeType> edgeTypes;
	// Preorder number for the node (-1 if unassigned)
	int pre = -1;
	// Postorder number for the node (-1 if unassigned)
	int post = -1;
public:
	Node() {}; // Default node constructor
	int getPreorder() { return pre; } // Getter for preorder number
	void setPreorder(int k) { pre = k; } // Setter for preorder number
	int getPostorder() { return post; } // Setter for postorder number
	void setPostorder(int k) { post = k; } // Setter for postorder number
	bool isVisited() { return (pre >= 0); } // Returns whether node has been visited
	int getChild(int k) { return children[k]; } // Gets vertex of child at index [k] of children
	int countChildren() { return children.size(); } // Get number of children of node
	void addChild(int n) { children.push_back(n); } // Adds [n] to vector of children
	// Sets edge from node to [v] as type [e]
	void setEdgeType(int v, EdgeType e) { edgeTypes.push_back(e); }

	EdgeType getEdgeType(int v);
};

// Gets type of edge from node to [v]
EdgeType Node::getEdgeType(int v) {
	vector<int>::iterator it = find(children.begin(), children.end(), v);
	return edgeTypes[it - children.begin()];
}

/***************************************************************************************
 * GRAPH CLASS DECLARATION
 ***************************************************************************************/
 // Declare class Graph
class Graph {
	// Vector of nodes composing the graph
	vector<Node> nodes;
	// Total number of nodes in the graph
	int size = 0;
	// Vector representing path taken by DFS, start to finish from left to right
	vector<int> DFSpath;
	// Vertex of first unvisited node, to track what the next tree should start from
	int smallestUnvisited = 0;
	// Vertex of root of current tree
	int currentRoot = 0;
	// Current value of preorder number
	int preorder = 0;
	// Current value of postorder number
	int postorder = 0;
public:
	Graph() {}; // Default constructor for Graph
	Graph(vector<Node> gnodes, int nodesSize);
	// Gets vertex of smallest unvisited node
	int getSmallestUnvisited() { return smallestUnvisited; }
	// Returns number of children of node [n]
	int countChildren(int n) { return nodes[n].countChildren(); }
	// Gets [c]th child of [n]
	int getChild(int n, int c) { return nodes[n].getChild(c); }
	// Returns whether [n] has been visited
	bool isVisited(int n) { return (n >= size || nodes[n].isVisited()); }
	// Returns whether [n] is root of current tree
	bool isCurrentRoot(int n) { return (n == currentRoot); }
	// Returns type of edge from [u] to [v]
	EdgeType getEdgeType(int u, int v) { return nodes[u].getEdgeType(v); }

	void updateSmallestUnvisited();
	void visit(int n);
	void leave(int n);
	void updateEdge(int u, int v);
	void printPath();
};

// Constructor for Graph with [size] nodes [gnodes]
Graph::Graph(vector<Node> gnodes, int nodesSize) {
	nodes = gnodes;
	size = nodesSize;
}

// Updates smallestUnvisited with next smallest unvisited value
void Graph::updateSmallestUnvisited() {
	smallestUnvisited++;
	for (int k = 0; k < nodes.size(); k++) {
		if (isVisited(smallestUnvisited)) { smallestUnvisited++; }
		else { break; }
	}
}

// Mark node [n] as visited by assigning preorder number and adding to DFS path
void Graph::visit(int n) {
	// Assign preorder number
	nodes[n].setPreorder(preorder);
	preorder++;
	// Add newly-visited node to path
	DFSpath.push_back(n);
	if (n == smallestUnvisited) {
		updateSmallestUnvisited();
	}
}

// Sets postorder number of [n] to current postorder number
void Graph::leave(int n) {
	nodes[n].setPostorder(postorder);
	postorder++;
}

// Sets type of edge from node [u] to [v]
void Graph::updateEdge(int u, int v) {
	int uPre = nodes[u].getPreorder();
	int vPre = nodes[v].getPreorder();
	EdgeType eType;

	// Identify edge type
	if (vPre < 0) { eType = Tree; }
	else { // v has been seen
		if (vPre > uPre) { eType = Forward; }
		else { // v has lower preorder
			if (nodes[v].getPostorder() >= 0) { eType = Cross; }
			else { eType = Back; }
		}
	}

	// Set edge as determined type
	nodes[u].setEdgeType(v, eType);
}

// Prints DFS path
void Graph::printPath() {
	printf("0");
	vector<int>::iterator it = DFSpath.begin();
	++it;
	for (it; it != DFSpath.end(); ++it) {
		printf(" %d", *it);
	}
	printf("\n");
}

/***************************************************************************************
 * HELPER FUNCTIONS
 ***************************************************************************************/
 // Parses input, storing graph and list of edges constructed from input
void buildGraph(Graph* gptr, vector<pair<int, int>>* eptr) {
	// Get initial input (n, m)
	int n, m;
	scanf("%d%d", &n, &m);

	// Get all subsequent inputs (u, v)
	int u, v;
	// Initialize nodes
	vector<Node> nodes; // Vector of already-initialized nodes
	vector<Node>* nodesptr = &nodes;

	nodesptr->assign(n, Node()); // Set vector to correct length, filled with empty nodes

	// Iterate through inputs, defining edges and saving in vector
	for (int k = 0; k < m; k++) {
		scanf("%d%d", &u, &v);
		eptr->emplace_back(u, v);
		nodes[u].addChild(v);
	}

	Graph g = Graph(nodes, n);
	*gptr = g;
}

// Recursive implementation of DFS, starting from node [n] in graph [graph]
void DFS(Graph * gptr, int n) {
	// Visit node n
	gptr->visit(n);

	// Normal DFS search
	int child;
	int childrenCount = gptr->countChildren(n);

	// Check all children of node
	for (int c = 0; c < childrenCount; c++) {
		child = gptr->getChild(n, c);

		// Update edge between both nodes
		gptr->updateEdge(n, child);

		if (!gptr->isVisited(child)) { DFS(gptr, child); }

	}

	// After exploring all subtrees, assign postorder number
	gptr->leave(n);

	// If new tree is needed, since DFS has returned to root but still unvisited nodes
	int smallestUnvisited = gptr->getSmallestUnvisited();
	if (gptr->isCurrentRoot(n)) {
		while (!gptr->isVisited(smallestUnvisited)) {
			DFS(gptr, smallestUnvisited);
			smallestUnvisited = gptr->getSmallestUnvisited();
		}
	}
}

// Generates and prints the output in proper format from having run DFS on [graph]
void output(Graph * gptr, vector<pair<int, int>>* eptr) {
	// Print path
	gptr->printPath();

	// Print edges
	vector <pair<int, int>> edges = *eptr;
	int u, v;
	int ind = 0;

	for (vector <pair<int, int>>::iterator it = edges.begin(); it != edges.end(); ++it) {
		u = get<0>(*it);
		v = get<1>(*it);

		printf("%d %d ", u, v);
		switch (gptr->getEdgeType(u, v)) {
		case Undefined:
			printf("?");
			break;
		case Tree:
			printf("t");
			break;
		case Forward:
			printf("f");
			break;
		case Back:
			printf("b");
			break;
		case Cross:
			printf("c");
			break;
		}
		printf("\n");
		ind++;
	}
}

/***************************************************************************************
 * MAIN FUNCTION
 ***************************************************************************************/
// Executes our problem solution
int main() {
	// Initialize graph
	Graph g;
	Graph* gptr = &g;

	// Initialize vector of edges
	vector <pair<int, int>> e;
	vector <pair<int, int>>* eptr = &e;

	// Actual execution
	buildGraph(gptr, eptr);
	DFS(gptr, 0);
	output(gptr, eptr);
}