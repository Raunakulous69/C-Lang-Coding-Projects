/***
* Raunak Anwar
* ranwar3
* 2025 Winter CSE101 PA2  
* FindPath.c
* Main Source File (Client Program) 
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
               addEdge(G, u, v); 
               
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
     printGraph(out, G);
     
     // in a loop, read the source and destination vertices and print the 
     // shortest path between them
     done = false;
     while (!done)
     {
          rc = fscanf(in, "%d %d", &u, &v);
          
          if (rc != 2)
          {
               fprintf(stderr,"Error:Couldn't read source & destination\n");
               fclose(in);
               fclose(out);
               freeGraph(&G);
               exit(EXIT_FAILURE);
          }
          
          else if (u == 0 && v == 0)
          {
               done = true;
          }
          
          else
          {
               BFS(G, u);
               List L = newList();
               getPath(L, G, v);
               fprintf(out, "\n");
               
               // if there are no path from u to v
               if (front(L) == NIL)
               {
                     fprintf(out, "The distance from %d to %d is infinity\n", u, v);
                     fprintf(out, "No %d-%d path exists\n", u, v);      
               }
               
               else 
               {
                    fprintf(out, "The distance from %d to %d is %d\n", u, v, getDist(G, v));
                    fprintf(out, "A shortest %d-%d path is: ", u, v);   
                    
                    printList(out, L);
                    
                    fprintf(out, "\n"); 
               }
               
               freeList(&L);
          }       
     }
     
     // cleaning up the graph
     fclose(in);
     fclose(out);
     freeGraph(&G); 
     
     return 0;
}
