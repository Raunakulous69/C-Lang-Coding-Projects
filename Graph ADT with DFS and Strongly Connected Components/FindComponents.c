/***
* Raunak Anwar
* ranwar3
* 2025 Winter CSE101 PA3  
* FindComponents.c
* Main Source File
***/ 

#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "List.h"

void usage(char *exc)
{
    fprintf(stderr,
         "SYNOPSIS:\n"
         "  Read the content of a given text file to create a graph\n"
         "  and find shortest paths between given nodes to the given\n"    
         "  output file\n"
         "\n"
         "USAGE:\n"
         "  %s <input file> <output file>\n"
         "\n"
         "OPTIONS:\n"
         "  <input file> Text file containing the input\n"
         "  <output file> Text file containing the output\n",
         exc);         
}

int main(int argc, char *argv[])
{
     // checking for 2 command-line arguments
     if (argc != 3)
     {
          usage(argv[0]);
          exit(EXIT_FAILURE);     
     }
     
     // opening the input file
     FILE *in = fopen(argv[1], "r");
     
     if (in == NULL)
     {
          fprintf(stderr, "Error: Failed to open input file: '%s'\n",argv[1]);
          
          exit(EXIT_FAILURE); 
     }
     
     // opening the output file
     FILE *out = fopen(argv[2], "w");
     
     if (out == NULL)
     {
          fprintf(stderr, "Error: Failed to open output file: '%s'\n",argv[2]);
          
          fclose(in);
          
          exit(EXIT_FAILURE); 
     }
     
     
     // Reading and storing the graph
     int n = 0; // number of vertices
     
     int rc = fscanf(in, "%d", &n);
     
     if (rc != 1) 
     {
          fprintf(stderr, "Error: Failed to read number of vertices\n");
          fclose(in);
          fclose(out);
          exit(EXIT_FAILURE);
     }
     
     // Creating the graph and reading its adjacency list
     Graph G = newGraph(n);
     int u,v;
     bool done = false;
     
     while (!done)
     {
          rc = fscanf(in, "%d %d", &u, &v);
          
          if (rc != 2)
          {
               fprintf(stderr, "Error:Adjacency list doesnt' have 2 #'s\n");
               fclose(in);
               fclose(out);
               freeGraph(&G);
               exit(EXIT_FAILURE);
          }
          
          if (u >= 1 && u <= n && v >=1 && v <= n)
          {
               addArc(G, u, v); 
               
          }
          
          else if (u == 0 && v == 0)
          {
               done = true;
          }
          
          else 
          {
               fprintf(stderr, "Error:Failed to read adjacency list\n");
               fclose(in);
               fclose(out);
               freeGraph(&G);
               exit(EXIT_FAILURE);
          }
     
     }
     
     // Print the graph to the output file
     fprintf(out, "Adjacency list representation of G:\n");
     printGraph(out, G);
     
     // Create a list for sending the list to the vertices
     List S = newList();
     for (int i = 1; i <= n; i++)
     {
          append(S, i);
     }
     
     // printf("Debug 1\n");
     
     // run the DFS to collect into the list S, all the vertices in 
     // decreasing finishing time order
     DFS(G,S);
     
     // printf("Debug 2\n");
     
     // Getting the transpose of G and running the DFS a 2nd time
     Graph T = transpose(G);
     
     // printf("Debug 3\n");
     
     DFS(T,S);
     
     // printf("Debug 4\n");
     
     // helps record the number of strong components in G
     int counter = 0;
     
     // in the transpose graph, count how many vertices have parents that 
     // are NIL
     for (int j = 1; j <= n; j++)
     {
          if (getParent(T,j) == NIL)
          {
               counter += 1;
          }
     } 
     
     // printf("Debug 5\n");
     
     fprintf(out, "\nG contains %d strongly connected components:\n", counter);
     
     
     // create a new list to hold a connected component
     List L = newList();
     
     moveBack(S);
     int component_number = 0;
     
     while(index(S) >= 0)
     {
          int curr = get(S);
          prepend(L, curr);
          
          if (getParent(T, curr) == NIL)
          {
               component_number += 1;
               fprintf(out, "Component %d: ", component_number);
               printList(out, L);
               fprintf(out, "\n"); 
               clear(L);
          }
          
          movePrev(S);
     }
     
     // printf("Debug 6\n");
     
     
     // cleaning up the graph and lists
     fclose(in);
     fclose(out);
     freeGraph(&G); 
     freeGraph(&T);
     freeList(&S);
     freeList(&L);
     
     // printf("Debug 7\n");
     
     return 0;
}
