/***
* Raunak Anwar
* ranwar3
* 2025 Winter CSE101 PA3  
* Graph.c
* Source File with graph functions
***/ 

#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "List.h"

#define WHITE 0
#define GRAY 1
#define BLACK 2

// Struct definition for GraphObj
typedef struct GraphObj {
     List *neighbors; // An array of Lists whose 𝑖th element contains the 
                      // neighbors of vertex 𝑖 (adj list representation)
     
     int *color; // An array of ints (or chars, or strings) whose 𝑖th element 
                  // is the color (white, gray, black) of vertex 𝑖
     
     int *parent; // An array of ints whose 𝑖th element is the parent of 
                  // vertex 𝑖
     
     int order; // number of vertices
     
     int size; // number of edges
     
     int *discover; // time at which a vertex is 1st visited during DFS
     
     int *finish; // time at which DFS finishes processing a vertex and its 
                 // descendants
     
} GraphObj;

// Constructors-Destructors
Graph newGraph(int n)
{
     // allocating memory for the GraphObj
     Graph G = malloc(sizeof(GraphObj));
     G->order = n;
     G->size = 0;
     
     int m = n + 1;
     
     // allocating memory for the states of the graph
     G->neighbors = calloc(m, sizeof(List));
     G->color = calloc(m, sizeof(int));
     G->parent = calloc(m, sizeof(int));
     G->discover = calloc(m, sizeof(int));
     G->finish = calloc(m, sizeof(int));     
     
     // initializing all the states of the graph
     for (int i = 1; i < m; i++) 
     {
          G->neighbors[i] = newList();
          G->color[i] = WHITE;
          G->parent[i] = NIL;
          G->discover[i] = UNDEF;
          G->finish[i] = UNDEF;          
     }
     
     return G;
}


void freeGraph(Graph* pG)
{
     if (pG == NULL || *pG == NULL)
     {
          return;
     }
     
     Graph G = *pG;
     
     int m = G->order + 1;
     
     for (int i = 1; i < m; i++)
     {
          freeList(&(G->neighbors[i]));     
     }
     
     free(G->neighbors);
     free(G->color);
     free(G->parent);
     free(G->discover);
     free(G->finish);
     
     G->neighbors = NULL;
     G->color = NULL;
     G->parent = NULL;
     G->discover = NULL;
     G->finish = NULL;
     
     free(G);
     *pG = NULL;
} 


// Access Functions
int getOrder(Graph G)
{
     return G->order;
} 

int getSize(Graph G)
{
     return G->size;
} 

int getParent(Graph G, int u)
{
     if (u < 1 || u > getOrder(G))
     {
          fprintf(stderr, "Graph Error: vertex is invalid\n");
          exit(EXIT_FAILURE);  
     }
     
     return G->parent[u];
} 

int getDiscover(Graph G, int u)
{
     if (u < 1 || u > getOrder(G))
     {
          fprintf(stderr, "Graph Error: vertex is invalid\n");
          exit(EXIT_FAILURE);  
     }
     
     return G->discover[u];
}     
     

int getFinish(Graph G, int u)
{
     if (u < 1 || u > getOrder(G))
     {
          fprintf(stderr, "Graph Error: vertex is invalid\n");
          exit(EXIT_FAILURE);  
     }
     
     return G->finish[u];
}  


// Manipulation Procedures
void addArc(Graph G, int u, int v)
{
     if (u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G))
     {
          fprintf(stderr, "Graph Error: int arguments is out of range\n");
          exit(EXIT_FAILURE);
     }
     
     List L = G->neighbors[u];
     
     if (length(L) == 0)
     {
          append(L,v);
     }
     
     else
     {
          // check if v is already in the adjacency list of u
          moveFront(L);
          while (index(L) >= 0)
          {
               if (get(L) == v)
               {
                    // v is already there
                    return;
               }
               
               moveNext(L);
          }
          
          // add v to u's adjacency list
          moveBack(L);
          int j = index(L);
          
          while (j >= 0 && get(L) > v)
          {
               movePrev(L);
               j = index(L);
          }
          
          if (j < 0)
          {
               prepend(L, v);
          }
          
          else 
          {
               insertAfter(L, v);
          }
     }
     
     G->size += 1;
} 

void addEdge(Graph G, int u, int v)
{
     if (u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G))
     {
          fprintf(stderr, "Graph Error: int arguments is out of range\n");
          exit(EXIT_FAILURE);
     }
     
     // if the 2 vertices are the same, they only need one arc
     if (u == v)
     {
          addArc(G, u, v);
     }
     
     // edge can be bidirectional which is why addArc is done twice
     else
     {
          addArc(G,u,v);
          addArc(G,v,u);
     }
     
     G->size -= 1;
}  


void DFS_Visit(Graph G, int u, int *time)
{
     // u is the vertex that we are currently visting on the Graph
     // update the states of u at the beginning of the visit
     *time = (*time) + 1;
     G->discover[u] = *time;
     G->color[u] = GRAY;
     
     // visit the neighbor of u
     List L = G->neighbors[u];
     if (length(L) > 0)
     {
          moveFront(L);
          while (index(L) >= 0)
          {
               int v = get(L);
               
               if (G->color[v] == WHITE)
               {
                    G->parent[v] = u;
                    DFS_Visit(G, v, time);
               }
               
               moveNext(L);
          }
     }
     
     // update the space of u at the end of the visit
     *time = (*time) + 1;
     G->finish[u] = *time;
     
     // after being done with u
     G->color[u] = BLACK;
}


void DFS(Graph G, List S)
{
     if (length(S) != getOrder(G))
     {
          fprintf(stderr, "Graph Error: wrong list size in DFS\n");
          exit(EXIT_FAILURE);  
     }
     
     int m = getOrder(G) + 1;
     
     // initializing the properties for DFS
     for (int i = 1; i < m; i++)
     {
          G->color[i] = WHITE;
          G->parent[i] = NIL;
          G->discover[i] = UNDEF;
          G->finish[i] = UNDEF;
     }
     
     // variable to track time
     int time = 0;
     
     // DFS 1 for each vertex that's given in List S
     moveFront(S);
     
     // iterate over the list
     while(index(S) >= 0)
     {
          int u = get(S);
          
          if (G->color[u] == WHITE)
          {
               DFS_Visit(G, u, &time);
          }
          
          moveNext(S);
      
     }
     
     // creating a new list to hold the vertices in decreasing finishing 
     // time
     List L = newList();
     
     moveFront(S);
     
     while (index(S) >= 0)
     {
          // get current item from list S
          int v = get(S);
          
          // insert v into the list L in descending order of finishing time
          moveBack(L);
          int j = index(L);
          
          while (j >= 0 && G->finish[get(L)] < G->finish[v])
          {
               movePrev(L);
               j = index(L);
          }
          
          // v is the largest vertex that's inserted at beginning of list
          if (j < 0)
          {
               prepend(L, v);
          }
          
          else 
          {
               insertAfter(L, v);
          }     
          
          moveNext(S);
          
     } // end of while
     
     // copy all items from List L and put it into List S
     moveFront(L);
     moveFront(S);
     
     while (index(L) >= 0)
     {
          set(S, get(L));
          moveNext(S);
          moveNext(L);
     }
     
     freeList(&L);
     
}
 

// Other Functions
Graph transpose(Graph G)
{
     int m = getOrder(G);
     Graph G1 = newGraph(m);
     
     int n = m + 1;
     for (int i = 1; i < n; i++)
     {
          // get the neighbor of vertex i
          List L = G->neighbors[i];
          
          // if vertex i has any neighbor
          if (length(L) > 0)
          {
               // move to the front of the neighbor's list
               moveFront(L);
               
               // add i to the adjacency list of each neighbor
               while (index(L) >= 0)
               {
                    int v = get(L);
                    addArc(G1, v, i);
                    moveNext(L);     
               } 
          }
          
          
          // copy the rest of the graph in its attributes
          G1->color[i] = G->color[i];
          G1->parent[i] = G->parent[i];
          G1->discover[i] = G->discover[i];
          G1->finish[i] = G->finish[i];   
     } 
     
     G1->size = G->size;
     G1->order = G->order;
     
     return G1;

} 

Graph copyGraph(Graph G)
{
     int m = getOrder(G);
     Graph G1 = newGraph(m);
     
     int n = m + 1;
     for (int i = 1; i < n; i++)
     {
          G1->neighbors[i] = copyList(G->neighbors[i]);
          G1->color[i] = G->color[i];
          G1->parent[i] = G->parent[i];
          G1->discover[i] = G->discover[i];
          G1->finish[i] = G->finish[i];   
     } 
     
     G1->size = G->size;
     G1->order = G->order;
     
     return G1;
}
 

void printGraph(FILE* out, Graph G)
{
     int m = G->order + 1;
     
     for (int i = 1; i < m; i++)
     {
          fprintf(out, "%d: ", i);
          printList(out, G->neighbors[i]);
          fprintf(out, "\n");
     }
}
 

