#ifndef _GRAPH
#define _GRAPH
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "List.h"

#define NIL 0
#define UNDEF -1

typedef struct GraphObj *Graph;

// Constructors-Destructors
Graph newGraph(int n); // returns a reference to a new graph object 
		       // containing n vertices and no edges

void freeGraph(Graph* pG); // frees all heap memory associated with a graph 
			   // and sets its Graph argument to NULL


// Access Functions
int getOrder(Graph G); // returns the number of vertices in G

int getSize(Graph G); // returns the number of edges in G 

int getParent(Graph G, int u); // return the appropriate field values for the 
			       // given vertex

int getDiscover(Graph G, int u); // return the appropriate field values for 
                                 // the given vertex
                                 // (Pre: 1 <= u <= n=getOrder(G))

int getFinish(Graph G, int u); // return the appropriate field values for the 
                               // given vertex
                               // (Pre: 1 <= u <= n=getOrder(G))


// Manipulation Procedures
void addArc(Graph G, int u, int v); // inserts a new directed edge from 𝑢 to 
				    // 𝑣, i.e. 𝑣 is added to the adjacency 
				    // List of 𝑢 (but not 𝑢 to the adjacency
				    // List of 𝑣). 
				    // has precondition that their two int 
				    // arguments must lie in the range 1 to 
				    // getOrder(G). 

void addEdge(Graph G, int u, int v); // inserts a new edge joining 𝑢 to 𝑣, 
				     // i.e. 𝑢 is added to the adjacency List 
				     // of 𝑣, and 𝑣 to the adjacency List of
				     // 𝑢. Your program is required to 	
				     // maintain these lists in sorted order 
				     // by increasing labels.
				     // has precondition that their two int 
				     // arguments must lie in the range 1 to 
				     // getOrder(G)

void DFS(Graph G, List S); //  graph traversal algorithm that explores as far 
                           // as possible along each branch before 
                           // backtracking. It is commonly used for tasks 
                           // such as finding connected components, detecting 
                           // cycles, and topological sorting


// Other Functions
Graph transpose(Graph G); // returns a reference to a new graph object 
                          // representing the transpose of G

Graph copyGraph(Graph G); // returns a reference to a new graph that is a 
                          // copy of G

void printGraph(FILE* out, Graph G); // prints the adjacency list 
                                     // representation of G to the file 
                                     // pointed to by out


#endif
