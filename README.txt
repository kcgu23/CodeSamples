# Code Samples

These code samples were written as homework assignments for class. We were only allowed to submit our solutions in a single file, so each full solution is located in each file. Our solutions were to be written in a language of our choice and I chose C++. These were individual assignments, so all code is exclusively my own work.

Brief descriptions of each problem that the code samples were meant to solve are included below. Each individual file includes more detailed problem specifications at the top, including: the expected inputs, the expected outputs, and the assumptions we were told to make.

## Alignment
This program computes the *edit distance* between two words, defined as the smallest number of single-letter insertions, deletions, or substitutions needed to change one word to another. In addition, it returns an optimal alignment, which is a way of aligning both words such that one can be changed to another in the same number of single-letter operations dictated by the edit distance. There can be multiple optimal alignments.

For example, the input

    abracadabra
    candelabra

has an optimal alignment of

     abracadabra
    candela  bra
    I SSSS DD   

We can see in the third line (where ``I`` represents an insertion, ``S`` represents a substitution, and ``D`` represents a deletion) that ``abracadabra`` can be changed to ``candelabra`` with 7 single-letter insertions/deletions/substitutions. This aligns (no pun intended) with the fact that the edit distance of these two words is 7.

We can also see that another optimal alignment for the same words is given by:

    abracad  abra
    c  an delabra
    SDD SD II    

Thus, we can return any optimal alignment, as long as it is still optimal.

## DFS_Labeling
This program implements a linear-time depth-first search (DFS) algorithm for a directed graph, labeling each edge as one of four types (tree, forward, back, cross) based on their traversal in the DFS search.

The edge types are determined by the following criteria:
* An edge *(u, v)* is a tree edge if it is the edge that you are traversing when visiting *v* for the first time in the DFS algorithm. The DFS forest is a set of rooted trees consisting of all the nodes in the graph and just the tree edges.
* An edge *(u, v)* is a forward edge if *v* is a descendant of *u* in the DFS forest, but *(u, v)* is not a tree edge; that is, there is a path of tree edges of length at least two from *u* to *v*.
* An edge *(u, v)* is a back edge if *v* is an ancestor of *u* in the DFS forest; that is, there exists a path from *v* to *u* of tree edges.
* An edge *(u, v)* is a cross edge in all other cases; that is, there is no path of tree edges from *u* to *v* nor from *v* to *u*.

## Villages
This problem posits that we are a road engineer for a remote region with *n* villages and *m* roads, where the villages in the region are currently not fully connected by roads. We want to build new roads so that every village in the region is accessible to every other village by a sequence of roads. Our program determines the minimum number of new roads needed for there to be full connectivity.