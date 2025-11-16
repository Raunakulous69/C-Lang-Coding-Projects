/***
* Raunak Anwar
* ranwar3
* 2025 Winter CSE101 PA4  
* Sparse.c
* Main Source File
***/ 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Matrix.h"
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
     
     
     // Reading and storing the Matrices
     int n; // total number of rows/columns for each matrix 
     
     int a; // number of non-zero elements in matrix A
    
     int b; // number of non-zero elements in matrix B
     
     fscanf(in, "%d %d %d",&n,&a,&b);

     int row = 0;
     int col = 0;
     double data = 0.0;
     
     // reading Matrix A
     Matrix A = newMatrix(n);
     for (int i = 0; i < a; i++)
     {
          fscanf(in, "%d %d %lf", &row, &col, &data); 
          changeEntry(A, row, col, data);     
     }
     
     
     // reading Matrix B
     Matrix B = newMatrix(n);
     for (int i = 0; i < b; i++)
     {
          fscanf(in, "%d %d %lf", &row, &col, &data); 
          changeEntry(B, row, col, data);
     }
     
     
     // NNZ for matrix A
     fprintf(out, "A has %d non-zero entries:\n", NNZ(A));
     printMatrix(out, A);
     fprintf(out, "\n"); 
     
     // NNZ for matrix B
     fprintf(out, "B has %d non-zero entries:\n", NNZ(B));
     printMatrix(out, B);
     fprintf(out, "\n"); 
     
     // Multiplication by 1.5 for matrix A
     Matrix A_1_5 = scalarMult(1.5, A);
     fprintf(out, "(1.5)*A =\n");
     printMatrix(out, A_1_5);
     fprintf(out, "\n");
     freeMatrix(&A_1_5);
     
     // Addition of matrix A and B
     Matrix sumAB = sum(A,B);
     fprintf(out, "A+B =\n");
     printMatrix(out, sumAB);
     fprintf(out, "\n");
     freeMatrix(&sumAB);
          
     // Addition of 2 Matrix A's
     Matrix sumAA = sum(A,A);
     fprintf(out, "A+A =\n");
     printMatrix(out, sumAA);
     fprintf(out, "\n");
     freeMatrix(&sumAA);
     
     // Subtraction of A from B
     Matrix diffAB = diff(B, A);
     fprintf(out, "B-A =\n");
     printMatrix(out, diffAB);
     fprintf(out, "\n");
     freeMatrix(&diffAB);
          
     // Subtraction of A from A
     Matrix diffAA = diff(A, A);
     fprintf(out, "A-A =\n");
     printMatrix(out, diffAA);
     fprintf(out, "\n");
     freeMatrix(&diffAA);
          
     // Transpose of Matrix A
     Matrix TA = transpose(A);
     fprintf(out, "Transpose(A) =\n");
     printMatrix(out, TA);
     fprintf(out, "\n");
     freeMatrix(&TA);
     
     // Multiplication of Matrix A and B
     Matrix multAB = product(A,B);
     fprintf(out, "A*B =\n");
     printMatrix(out, multAB);
     fprintf(out, "\n");
     freeMatrix(&multAB);
     
     // Multiplication of 2 Matrix B's
     Matrix multBB = product(B,B);
     fprintf(out, "B*B =\n");
     printMatrix(out, multBB);
     fprintf(out, "\n");
     freeMatrix(&multBB);
     
     
     // freeing the matrices
     freeMatrix(&A);
     freeMatrix(&B);

     
     // closing the files
     fclose(in);
     fclose(out);
   
     return 0;
}
