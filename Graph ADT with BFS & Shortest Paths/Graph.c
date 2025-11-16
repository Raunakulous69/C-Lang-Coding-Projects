/***
* Raunak Anwar
* ranwar3
* 2025 Winter CSE101 PA2  
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
                      // neighbors of vertex 𝑖.
     
     int *color; // An array of ints (or chars, or strings) whose 𝑖th element 
                  // is the color (white, gray, black) of vertex 𝑖
     
     int *parent; // An array of ints whose 𝑖th element is the parent of 
                  // vertex 𝑖
     
     int *distance; // An array of ints whose 𝑖th element is the distance 
                    // from the (most recent) source to vertex 𝑖.
     
     int order; // number of vertices
     
     int size; // number of edges
     
     int source; // the vertex that was most recently used as source for BFS
     
} GraphObj;



/*** Constructors-Destructors ***/
Graph newGraph(int n)
{
     // allocating memory for the GraphObj
     Graph G = malloc(sizeof(GraphObj));
     G->order = n;
     G->size = 0;
     G->source = NIL;
     
     int m = n + 1;
     
     // allocating memory for the states of the graph
     G->neighbors = calloc(m, sizeof(List));
     G->color = calloc(m, sizeof(int));
     G->parent = calloc(m, sizeof(int));
     G->distance = calloc(m, sizeof(int));
     
     // initializing all the states of the graph
     for (int i = 1; i < m; i++) 
     {
          G->neighbors[i] = newList();
          G->color[i] = WHITE;
          G->parent[i] = NIL;
          G->distance[i] = INF;
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
     free(G->distance);
     
     G->neighbors = NULL;
     G->color = NULL;
     G->parent = NULL;
     G->distance = NULL;
     
     free(G);
     *pG = NULL;
}


/*** Access functions ***/
int getOrder(Graph G)
{
     return G->order;
}

int getSize(Graph G)
{
     return G->size;
}

int getSource(Graph G)
{
     return G->source;
}

int getParent(Graph G, int u)
{
     if (u < 1 || u > getOrder(G))
     {
          fprintf(stderr, "Graph Error: vertex is invalid\n");
          exit(EXIT_FAILURE);  
     }
     
     // if BFS is not called, then source vertex of Graph will be NIL
     if (G->source == NIL)
     {
          return NIL;
     }
     
     return G->parent[u];
}

int getDist(Graph G, int u)
{
     if (u < 1 || u > getOrder(G))
     {
          fprintf(stderr, "Graph Error: vertex is invalid\n");
          exit(EXIT_FAILURE);  
     }
     
     // if BFS is not called, then source vertex of Graph will be NIL
     if (G->source == NIL)
     {
          return INF;
     }
     
     return G->distance[u];
}

void getPath(List L, Graph G, int u)
{
     if (u < 1 || u > getOrder(G))
     {
          fprintf(stderr, "Graph Error: vertex is invalid\n");
          exit(EXIT_FAILURE);  
     }
     
     // BFS must be called before getPath
     if (getSource(G) == NIL)
     {
          fprintf(stderr, "Graph Error: BFS has not been called before getPath()\n");
          exit(EXIT_FAILURE);
     }
     
     // when source and destination is the exact same thing, then the path
     // is just a single vertex
     if (u == G->source)
     {
          append(L,u);
          return;
     }
     
     // checking if there is a path from the source to u
     bool hasPath = false;
     int p = G->parent[u];
     
     while (p != NIL)
     {
          if (p == G->source)
          {
               hasPath = true; 
               break;
          }
          
          else 
          {
               p = G->parent[p];
          }
     }
     
     // if there is a path, then add each vertex of that path to the list
     if (hasPath)
     {
          while (u != G->source)
          {
               prepend(L,u);
               u = G->parent[u];
          }
          
          // u still needs to prepended to List once it equals source vertex
          prepend(L,u);
     }
     
     // if there is no path, then just append a NIL to the list
     else
     {
          append(L,NIL);
     } 
}


/*** Manipulation procedures ***/
void makeNull(Graph G)
{
     int m = G->order + 1;
     
     for (int i = 1; i < m; i++)
     {
          clear(G->neighbors[i]);
          G->color[i] = WHITE;
          G->parent[i] = NIL;
          G->distance[i] = INF; 
     }
     
     G->size = 0;
     G->source = NIL;
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

void BFS(Graph G, int s)
{
     // setting color to white (unvisited), distances to 0, and parents to 
     // -1
     for (int i = 1; i <= G->order; i++)
     {
          G->color[i] = WHITE;
          G->distance[i] = INF;
          G->parent[i] = NIL;
     }
     
     G->color[s] = GRAY;
     G->distance[s] = 0;
     G->source = s;
     
     // using list as a queue
     List l = newList();
     append(l, s);
     
     // Iterating as long as the queue is not empty (# of vertex > 0)
     while (length(l) > 0)
     {
          // dequeing the the next vertex
          int u = front(l);
          deleteFront(l);
     
          // Iterating through the neighbors of u 
          for (moveFront(G->neighbors[u]); 0 <= index(G->neighbors[u]); moveNext(G->neighbors[u])) {
               
               int v = get(G->neighbors[u]);
               if (G->color[v] == WHITE)
               {
                    G->color[v] = GRAY;
                    G->distance[v] = G->distance[u] + 1;
                    G->parent[v] = u;
                    append(l, v);
               }
          }
          
          // Vertex has been fully explored
          G->color[u] = BLACK;
     }
     
     // freeing the list
     freeList(&l);
}


/*** Other operations ***/
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





