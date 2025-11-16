#ifndef _GRAPH
#define _GRAPH
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "List.h"

#define INF -1
#define NIL 0

typedef struct GraphObj *Graph;


/*** Constructors-Destructors ***/
Graph newGraph(int n); // returns a Graph pointing to a newly created 
		       // GraphObj representing a graph having
		       // 𝑛 vertices and no edges

void freeGraph(Graph* pG); //  frees all heap memory associated with the 
			   // Graph *pG, then sets the handle *pG to NULL



/*** Access functions ***/
int getOrder(Graph G); // returns the order of the graph

int getSize(Graph G); // return the size of the graph

int getSource(Graph G); // returns the source vertex most recently used in 
                        // function BFS(), or NIL if BFS() has not yet been 
                        // called

int getParent(Graph G, int u); // return the parent of vertex 𝑢 in the BFS 
                               // tree created by BFS(), or NIL if BFS() has 
                               // not yet been called
			       // (has the precondition 1 ≤ 𝑢 ≤ getOrder(𝐺))

int getDist(Graph G, int u); // return distance between parent vertex and 
			     // current vertex u. returns INF if BFS() has 
			     // not been called yet 
			     // (has the precondition 1 ≤ 𝑢 ≤ getOrder(𝐺))

void getPath(List L, Graph G, int u); // appends to the List L the vertices 
				      // of a shortest path in G from source 
				      // to u, or appends to L the value NIL 
				      // if no such path exists (has the 
				      // precondition getSource(G)!=NIL, so 
				      // BFS() must be called before 
				      // getPath() is called)
				      // (has precondition that 1 ≤ 𝑢 ≤ 
				      // getOrder(𝐺))



/*** Manipulation procedures ***/
void makeNull(Graph G); //  deletes all edges of 𝐺, restoring it to its
			// original (no edge) state (turns into a NULL graph 

void addEdge(Graph G, int u, int v); // inserts a new edge joining 𝑢 to 𝑣, 
				     // i.e. 𝑢 is added to the adjacency List 
				     // of 𝑣, and 𝑣 to the adjacency List of
				     // 𝑢. Your program is required to 	
				     // maintain these lists in sorted order 
				     // by increasing labels.
				     // has precondition that their two int 
				     // arguments must lie in the range 1 to 
				     // getOrder(G). 

void addArc(Graph G, int u, int v); // inserts a new directed edge from 𝑢 to 
				    // 𝑣, i.e. 𝑣 is added to the adjacency 
				    // List of 𝑢 (but not 𝑢 to the adjacency
				    // List of 𝑣). 
				    // has precondition that their two int 
				    // arguments must lie in the range 1 to 
				    // getOrder(G). 

void BFS(Graph G, int s); // runs the BFS algorithm on the Graph 𝐺 with
                          // source 𝑠, setting the color, distance, parent, 
                          // and source fields of 𝐺 accordingly. It also has 
                          // the precondition that the int argument 𝑠 lies in 
                          // the range 1 to getOrder(G).


/*** Other operations ***/
void printGraph(FILE* out, Graph G); //  prints the adjacency list 
                                     //representation of 𝐺 to the file 
                                     // pointed to by out. The format of this 
                                     // representation should match the
                                     // above examples, so all that is 
                                     // required by the client is a single 
                                     // call to printGraph()

#endif
