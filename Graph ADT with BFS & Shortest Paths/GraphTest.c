/***
* Raunak Anwar
* ranwar3
* 2025 Winter CSE101 PA2  
* GraphTest.c
* Testing File for the graph's functions
***/ 

//-----------------------------------------------------------------------------
//
// GraphClient.c
// 
// This is a fancy test client that calculates the Radius and Diameter of 
// the graph defined on lines 41-49, along with its Central and Peripheral 
// vertices.  The definitions of these terms at:
// 
//    http://en.wikipedia.org/wiki/Distance_(graph_theory)
// 
// Place this file in a directory with copies of your List.c, List.h, Graph.c, 
// Graph.h and an appropriate Makefile, then compile and run. The output 
// is contained in the file GraphClientOut.
// 
// This program does not exercise all functions in your Graph ADT, but it 
// does a pretty good job of testing BFS().  If your output differs from 
// the above, you have a logical problem in either your Graph or List ADT.
// 
// Remember you are required to submit a file named GraphTest.c with pa4 that
// exercises your Graph functions.  Do not submit this file.
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Graph.h"

int main(int argc, char* argv[]){
   int i, s, max, min, d, n=35;
   List  C = newList(); // central vertices 
   List  P = newList(); // peripheral vertices 
   List  E = newList(); // eccentricities 
   Graph G = NULL;

   // Build graph G 
   G = newGraph(n);
   for(i=1; i<n; i++){
      if( i%7!=0 ) addEdge(G, i, i+1); //addEdge tests both addEdge & addArc
      if( i<=28  ) addEdge(G, i, i+7);
   }
   addEdge(G, 9, 31);
   addEdge(G, 17, 13);
   addEdge(G, 14, 33);

   // Print adjacency list representation of G
   printGraph(stdout, G);

   // Calculate the eccentricity of each vertex 
   for(s=1; s<=n; s++){
      BFS(G, s);
      max = getDist(G, 1);
      for(i=2; i<=n; i++){
         d = getDist(G, i);
         max = ( max<d ? d : max );
      }
      append(E, max);
   }

   // Determine the Radius and Diameter of G, as well as the Central and 
   // Peripheral vertices.
   append(C, 1);
   append(P, 1);
   min = max = front(E);
   moveFront(E);
   moveNext(E);
   for(i=2; i<=n; i++){
      d = get(E);
      if( d==min ){
         append(C, i);
      }else if( d<min ){
         min = d;
         clear(C);
         append(C, i);
      }
      if( d==max ){
         append(P, i);
      }else if( d>max ){
         max = d;
         clear(P);
         append(P, i);
      }
      moveNext(E);
   }
   
   
   // Print results 
   printf("\n");
   printf("Radius = %d\n", min);
   printf("Central vert%s: ", length(C)==1?"ex":"ices");
   printList(stdout, C);
   printf("\n");
   printf("Diameter = %d\n", max);
   printf("Peripheral vert%s: ", length(P)==1?"ex":"ices");
   printList(stdout, P);
   printf("\n");
   
      
   // testing for getOrder
   int order = getOrder(G);
   printf("The order of the graph is %d\n", order);   
   
   // testing for getSize
   int size = getSize(G);
   printf("The size of the graph is %d\n", size);
   
   // testing for getSource
   int source = getSource(G);
   assert (source == n);
   
   // testing for getParent
   int parent = getParent(G,source);
   assert (parent == NIL);
   
   // testing for getPath
   List L = newList();
   getPath(L, G, 20);
   printList(stdout, L);
   printf("\n");
   freeList(&L);
   
   
   // testing for makeNull
   makeNull(G);
   assert (getOrder(G) == n);
   assert (getSize(G) == 0);
   assert (getSource(G) == NIL);
   

   // Free objects 
   freeList(&C);
   freeList(&P);
   freeList(&E);
   freeGraph(&G);

   return(0);
}




/*
1: 2 8
2: 1 3 9
3: 2 4 10
4: 3 5 11
5: 4 6 12
6: 5 7 13
7: 6 14
8: 1 9 15
9: 2 8 10 16 31
10: 3 9 11 17
11: 4 10 12 18
12: 5 11 13 19
13: 6 12 14 17 20
14: 7 13 21 33
15: 8 16 22
16: 9 15 17 23
17: 10 13 16 18 24
18: 11 17 19 25
19: 12 18 20 26
20: 13 19 21 27
21: 14 20 28
22: 15 23 29
23: 16 22 24 30
24: 17 23 25 31
25: 18 24 26 32
26: 19 25 27 33
27: 20 26 28 34
28: 21 27 35
29: 22 30
30: 23 29 31
31: 9 24 30 32
32: 25 31 33
33: 14 26 32 34
34: 27 33 35
35: 28 34

Radius = 5
Central vertices: 13 17 18 24 25 31 32 33
Diameter = 8
Peripheral vertices: 1 28
*/
