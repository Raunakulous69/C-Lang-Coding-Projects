/***
* Raunak Anwar
* ranwar3
* 2025 Winter CSE101 PA4  
* Matrix.c
* Source File with matrix functions
***/ 

#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"
#include "List.h"

typedef struct EntryObj *Entry;

struct EntryObj {
     
     int col; // number of columns in matrix
              // no need for a row value since matrix already gives it
              // you're looking for the column value for item is located at
     
     double data; // value at (row, col) of the matrix
    
};

// Constructor-Destructor functions for Entry
Entry newEntry(int col, double data)
{
     Entry E = malloc(sizeof(struct EntryObj));
     E->col = col;
     E->data = data;
     
     return E;
}

void freeEntry(Entry* pE)
{
     if (pE == NULL || *pE == NULL)
     {
          return;
     }
     
     Entry E = *pE;
     
     free(E);
     *pE = NULL;
}

struct MatrixObj {
     
     List* rows; // list of entries that are maintained in column sorted 
                    // order
     
     int dimension; // dimension of the square matrix (n)
     
     int nnz; // number of non-zero elements in the matrix  
};



Matrix newMatrix(int n)
{
     Matrix M = malloc(sizeof(struct MatrixObj));
     M->rows = calloc(n+1, sizeof(List)); // n+1 b/c index 0 isn't used
     
     int m = n + 1;
     for (int i = 1; i < m; i++)
     {
          M->rows[i] = newList();         
     } 
     
     M->dimension = n;
     M->nnz = 0; // equals to 0 b/c there are 0 elements in all lists 
     
     return M;       
}

void freeMatrix(Matrix* pM)
{
     if (pM == NULL || *pM == NULL)
     {
          return;
     }
     
     Matrix M = *pM;
   
     int m = size(M) + 1;
     
     for (int i = 1; i < m; i++)
     {
          List L = M->rows[i];
          freeList(&L);
     }
     
     free(M->rows);
     M->rows = NULL;
     
     free(M);
     *pM = NULL;
}


// Access functions
int size(Matrix M)
{
     return M->dimension;  
} 

int NNZ(Matrix M)
{
     return M->nnz;
} 

int equals(Matrix A, Matrix B)
{
     if (size(A) != size(B) || NNZ(A) != NNZ(B))
     {
          return 0;
     }
     
     int m = size(A) + 1;
     
     for (int i = 1; i < m; i++)
     {
          List rowA = A->rows[i];
          List rowB = B->rows[i];
          
          if (length(rowA) != length(rowB))
          {
               return 0;
          }
          
          moveFront(rowA);
          moveFront(rowB);
          
          // compares current row from A to current row from B
          while (index(rowA) >= 0 && index(rowB) >= 0)
          {
               Entry entryA = (Entry) get(rowA);
               Entry entryB = (Entry) get(rowB);
               
               if (entryA->col != entryB->col || entryA->data != entryB->data) {
                    return 0;
               }
               
               moveNext(rowA);
               moveNext(rowB);
          }
     }
     
     return 1;     
} 


// Manipulation procedures
void makeZero(Matrix M)
{
     int m = (M->dimension) + 1;
     for (int i = 1; i < m; i++)
     {
          clear(M->rows[i]);    
     }
     
     M->nnz = 0;
} 

void changeEntry(Matrix M, int i, int j, double x)
{
     if (i < 1 || i > size(M) || j < 1 || j > size(M))
     {
          fprintf(stderr, "Error Message: column and/or row value is out of range\n");
          exit(EXIT_FAILURE);
     }
     
     // getting the row of the matrix where given entry belongs
     List row = M->rows[i];
     
     if (length(row) == 0)
     {
          if (x != 0.0)
          {
               Entry E = newEntry(j,x);
               append(row, E);
               M->nnz += 1;
          }
          
          return;    
     }
     
     moveFront(row);
     
     while (index(row) >= 0)
     {
          Entry E = (Entry) get(row);
          
          // entry already exists
          if (j == E->col)
          {
               // if x is 0, delete this entry and decrease NNZ by 1
               if (x == 0.0)
               {
                   //freeEntry(&E);
                   delete(row);
                   M->nnz -= 1; // column originally existed as a NNZ 
                                // element and now gets deleted b/c it is
                                // now a zero element             
               }
               
               else 
               {
                    E->data = x;     
               } 
               return;            
          }  
          
          // reaching a column that is greater than j, which means that a 
          // new entry has to be added before this column
          else if (j < E->col)
          {
               if (x != 0.0)
               {
                    Entry E1 = newEntry(j,x);
                    insertBefore(row,E1);
                    M->nnz += 1;
               }
               return;   
          }
          
          else 
          {
               moveNext(row);    
          }
           
     }
     
     // at this point, all the entries in the row have been checked 
     // and cursor reached the end of the list, so append a new entry
     if (x != 0)
     {
          Entry E2 = newEntry(j,x);
          append(row,E2);
          M->nnz += 1;
     }
}


// Matrix Arithmetic operations
Matrix copy(Matrix A)
{    
     return scalarMult(1.0,A);
} 

Matrix transpose(Matrix A)
{
     int m = size(A) + 1;
     Matrix B = newMatrix(size(A));
     
     for (int i = 1; i < m; i++)
     {
          List rowA = A->rows[i];
          moveFront(rowA);
          
          // traversing through the row itself and turning it into a column
          while(index(rowA) >= 0)
          {
               Entry E = get(rowA);
               int currRow = i;
               int currCol = E->col;
               double currVal =  E->data;
               
               Entry F = newEntry(currRow, currVal);
               
               append(B->rows[currCol], F);
               
               moveNext(rowA);
          }
     }
     
     B->dimension = A->dimension;
     B->nnz = A->nnz;
     
     return B;
} 
                            
                            
Matrix scalarMult(double x, Matrix A)
{
     int m = size(A) + 1;
     Matrix B = newMatrix(size(A));
     
     // must return a zero matrix if multiplied by 0
     if (x == 0)
     {
          return B;
     }
     
     for (int i = 1; i < m; i++)
     {
          List rowA = A->rows[i];
          moveFront(rowA);
          
          while(index(rowA) >= 0)
          {
               Entry E = get(rowA);
               Entry F = newEntry(E->col, E->data * x);
               
               append(B->rows[i], F);
               moveNext(rowA);
          }
     }
     
     B->dimension = A->dimension;
     B->nnz = A->nnz;
     
     return B;
} 
                                       

Matrix sum(Matrix A, Matrix B)
{
     if (size(A) != size(B))
     {
          fprintf(stderr, "Error Message: Matrices have different sizes\n");
          exit(EXIT_FAILURE);
     }
     
     if (A == B)
     {
          return scalarMult(2.0, A);
     }
     
     int m = size(A) + 1;
     Matrix C = newMatrix(size(A));
     
     for (int i = 1; i < m; i++)
     {
          List rowA = A->rows[i];
          List rowB = B->rows[i];
          
          moveFront(rowA);
          moveFront(rowB);
          
          while (index(rowA) >= 0 && index(rowB) >= 0)
          {
               Entry E = get(rowA);
               Entry F = get(rowB);
               
               if (E->col == F->col)
               {
                    double data = E->data + F->data;
                    changeEntry(C, i, E->col, data);
                    moveNext(rowA);
                    moveNext(rowB);
               }
               
               else if (E->col < F->col)
               {
                    changeEntry(C, i, E->col, E->data);
                    moveNext(rowA);
               }
               
               else 
               {
                    changeEntry(C, i, F->col, F->data);
                    moveNext(rowB);
               }                  
          }
          
          // process the leftover items from rowA
          while (index(rowA) >= 0)
          {
               Entry E = get(rowA);
               changeEntry(C, i, E->col, E->data);
               moveNext(rowA);
          }
          
          // process the leftover items from rowB
          while (index(rowB) >= 0)
          {
               Entry F = get(rowB);
               changeEntry(C, i, F->col, F->data);
               moveNext(rowB);
          }
          
     }
     
     // only update dimension value b/c changeEntry updates nnz for us
     C->dimension = A->dimension;
     
     return C;
}           


Matrix diff(Matrix A, Matrix B)
{
     if (size(A) != size(B))
     {
          fprintf(stderr, "Error Message: Matrices have different sizes\n");
          exit(EXIT_FAILURE);
     }
     
     if (A == B)
     {
          Matrix M = newMatrix(size(A));
          return M;
     }
     
     Matrix C = scalarMult(-1.0, B);
     Matrix D = sum(A,C);
     freeMatrix(&C);
     
     return D;
}      
 
                                 
double vectorDot(List P, List Q) // private function (used for product)
{
     double result = 0.0;
     
     moveFront(P);
     moveFront(Q);
     
     while(index(P)>= 0 && index(Q) >= 0)
     {
          Entry E = get(P);
          Entry F = get(Q);
          
          if (E->col == F->col)
          {
               result += E->data * F->data;
               
               moveNext(P);
               moveNext(Q);
          }
          
          else if (E->col < F->col)
          {
               moveNext(P);
          }
          
          else
          {
               moveNext(Q);
          }
     }
     
     return result;
}


Matrix product(Matrix A, Matrix B)
{
     if (size(A) != size(B))
     {
          fprintf(stderr, "Error Message: Matrices have different sizes\n");
          exit(EXIT_FAILURE);
     }
     
     int m = size(A) + 1;
     Matrix T = transpose(B);
     Matrix R = newMatrix(size(A));
     
     for (int i = 1; i < m; i++)
     {
          List rowA = A->rows[i];
          
          for (int j = 1; j < m; j++)
          {
               List rowT = T->rows[j];
               
               double data = vectorDot(rowA, rowT);
               
               if (data != 0.0)
               {
                    changeEntry(R,i,j,data);
               }      
          }  
     }
     
     freeMatrix(&T);
     
     return R;
}     
                                    

void printMatrix(FILE* out, Matrix M)
{
     int m = (M->dimension) + 1;
     
     for (int i = 1; i < m; i++)
     {
          
          List rowM = M->rows[i];
          
          moveFront(rowM);
          
          if (length(rowM) > 0)
          {
               fprintf(out, "%d:", i);
               while (index(rowM) >= 0)
               {
                    Entry E = get(rowM);
                    fprintf(out, " (%d, %.1lf)", E->col, E->data);
                    moveNext(rowM);  
               }
          
               fprintf(out, "\n");
          }
     }
} 
