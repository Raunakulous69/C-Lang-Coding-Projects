/***
* Raunak Anwar
* ranwar3
* 2025 Winter CSE101 PA3  
* List.c
* Source File with List Functions 
***/
#include "List.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct NodeObj *Node;

struct NodeObj {
	int data;
	Node next;
	Node prev;
};

struct ListObj {
	int length;
	int index;
	Node front;
	Node back;
	Node cursor;
};

// constructor for node
Node newNode(int data)
{
     Node n = (Node) malloc(sizeof(struct NodeObj));
	
     // if malloc can't allocate any memory at all, then n will be null
     if (n == NULL){
     	return NULL;
      }
	
      // data, prev, and next are members of the NodeObj structure
      n->data = data;
      n->prev = NULL;
      n->next = NULL;
      return n; 
}

// destructor for node 
void freeNode(Node *pN) // Node *pN is a pointer to a pointer to NodeObj
{
     if (pN != NULL && *pN != NULL)
     {
          Node n = *pN;
          free(n);
          *pN = NULL;
     }     
} 


// Constructors-Destructors

List newList(void) 
{
     List l = (List) malloc(sizeof(struct ListObj));
	
     // if malloc can't allocate any memory at all, then l will be null
     if (l == NULL){
     	return NULL;
      }
	
      // length, index, front, back, and cursor are members of the List 
      // structure (only member)
      l->length = 0;
      l->index = -1;
      l->front = NULL;
      l->back = NULL;
      l->cursor = NULL;
      return l;
}
			 

void freeList(List* pL)
{
     if (pL != NULL && *pL != NULL)
     {
          List l = *pL;
          
          // Free the nodes first
          Node currNode = l->front;
          while (currNode != NULL)
          {
               Node nextNode = currNode->next;
               freeNode(&currNode);
               currNode = nextNode;
          }
          // free the list 
          free(l);
          *pL = NULL;  
     }
} 
 			 

// Access Functions

int length(List L)
{
     return L->length;
} 

int index(List L)
{
    return L->index;  
} 

int front(List L)
{
     if (L == NULL || L->length < 1) 
     {
          fprintf(stderr, "List error: Null or empty list\n");
          exit(EXIT_FAILURE);
     }
     
     return L->front->data;
}

int back(List L)
{
     if (L == NULL || L->length < 1) 
     {
          fprintf(stderr, "List error: Null or empty list\n");
          exit(EXIT_FAILURE);
     }
     
     return L->back->data;
}

int get(List L)
{
     if (L == NULL || L->length < 1 || L->index < 0) 
     {
          fprintf(stderr, "List error: Null or empty list or undefined cursor\n");
          exit(EXIT_FAILURE);
     }
     
     return L->cursor->data;
}

bool equals(List A, List B) 
{
     // Check for Null
     if (A == NULL || B == NULL)
     {
          return false;
     }
     
     // Check for size of each list
     if (A->length != B->length)
     {
          return false;
     }
     
     // Check for each element of both lists
     Node nodeA = A->front;
     Node nodeB = B->front;
     
     while (nodeA != NULL && nodeB != NULL)
     {
          if (nodeA->data != nodeB->data)
          {
               return false;
          }  
          nodeA = nodeA->next;
          nodeB = nodeB->next;   
     }
     
     return true;
}



// Manipulation Procedures

void clear(List L)
{
     Node currNode = L->front;
     while (currNode != NULL)
     {
          Node nextNode = currNode->next;
          freeNode(&currNode);
          currNode = nextNode;
     }
     
     L->length = 0;
     L->index = -1;
     L->front = NULL;
     L->back = NULL;
     L->cursor = NULL;
} 

void set(List L, int x)
{
     if (L == NULL || L->length < 1 || L->index < 0) 
     {
          fprintf(stderr, "List error: Null or empty list or undefined cursor\n");
          exit(EXIT_FAILURE);
     }
     
     L->cursor->data = x;
} 

void moveFront(List L)
{
     if (L != NULL && L->length > 0)
     {
          L->cursor = L->front;
          L->index = 0;
     }
} 

void moveBack(List L)
{
     if (L != NULL && L->length > 0)
     {
          L->cursor = L->back;
          L->index = L->length - 1;
     }
}  

void movePrev(List L)
{ 
     if (L != NULL && L->index >= 0)
     {
          if (L->index == 0)
          {
               L->index = -1;
               L->cursor = NULL;
          }
          
          else
          {
               L->cursor = L->cursor->prev;
               L->index = L->index - 1;
          }
     }
}

void moveNext(List L)
{ 
     if (L != NULL && L->index >= 0)
     {
          if (L->index == (L->length - 1))
          {
               L->index = -1;
               L->cursor = NULL;
          }
          
          else
          {
               L->cursor = L->cursor->next;
               L->index = L->index + 1;
          }
     }
}

void prepend(List L, int x)
{
     if (L == NULL)
     {
          fprintf(stderr, "List Error: cannot prepend to a null list\n");
          exit(EXIT_FAILURE);
     }
     
     Node n = newNode(x);
     
     if (L->length == 0)
     {
          L->front = n;
          L->back = n;
     }
     
     else
     {
          // make current front the new node's next
          n->next = L->front;
          
          // assign new node to current front's prev
          L->front->prev = n;
          
          // make the new node the current front 
          L->front = n;
          
          // adjust the cursor's index if necessary
          if (L->index >= 0)
          {
               L->index += 1;
          }
     }
     
     L->length += 1;     
}               

void append(List L, int x)
{
     if (L == NULL)
     {
          fprintf(stderr, "List Error: cannot append to a null list\n");
          exit(EXIT_FAILURE);
     }
     
     Node n = newNode(x);
     
     if (L->length == 0)
     {
          L->front = n;
          L->back = n;
     }
     
     else
     {
          // make current back the new node's prev
          n->prev = L->back;
          
          // assign new node to current back's next
          L->back->next = n;
          
          // make the new node the current back 
          L->back = n;
     }
     
     L->length += 1;     
}               


void insertBefore(List L, int x)
{
     if (L == NULL || L->length < 1 || L->index < 0) 
     {
          fprintf(stderr, "List error: Null or empty list or undefined cursor\n");
          exit(EXIT_FAILURE);
     }
     
     // when the cursor is at the front
     if (L->index == 0)
     {
          prepend(L,x);  
     }
     
     else
     {
          Node n = newNode(x);
          
          // assigns List cursor to the next node after the new node
          n->next = L->cursor;
          
          // assigns the previous node before the List cursor to the 
          //previous node before the new node
          n->prev = L->cursor->prev;
          
          // new node is assigned to the next of the cursor's prev node
          n->prev->next = n;
          
          // making the new node the cursor's prev node
          n->next->prev = n;
          
          L->length += 1;
          L->index += 1;
     }
}

void insertAfter(List L, int x)
{
     if (L == NULL || L->length < 1 || L->index < 0) 
     {
          fprintf(stderr, "List error: Null or empty list or undefined cursor\n");
          exit(EXIT_FAILURE);
     }
     
     // when the cursor is at the back
     if (L->index == (L->length - 1))
     {
          append(L,x);  
     }
     
     else
     {
          Node n = newNode(x);
          
          // assigns the next node after the List cursor to the next node 
          // after the new node
          n->next = L->cursor->next;
          
          // assigns the  node of the List cursor to the previous node 
          // before the new node
          n->prev = L->cursor;
          
          // new node is assigned to the next of the cursor's prev node
          n->prev->next = n;
          
          // making the new node to the prev of the cursor's next node
          n->next->prev = n;
          
          L->length += 1;
     }
}


void deleteFront(List L)
{
     if (L == NULL || L->length < 1) 
     {
          fprintf(stderr, "List Error: can't delete front of an empty list\n");
          exit(EXIT_FAILURE);
     }
     
     Node n = L->front;
     
     if (L->length == 1)
     {
           L->front = NULL;
           L->back = NULL;
           L->cursor = NULL;
           L->index = -1;
     }
     
     else
     {
          L->front = L->front->next;
          L->front->prev = NULL;
          
          if (L->index == 0)
          {
              L->cursor = NULL;
              L->index = -1;
          } 
          
          else if (L->index > 0)
          {
               L->index -= 1;
          }         
     }
     
     // actually deleting the node
     freeNode(&n);
     
     // adjusting the length    
     L->length -= 1;
} 


void deleteBack(List L)
{
     if (L == NULL || L->length < 1) 
     {
          fprintf(stderr, "List Error: can't delete front of an empty list\n");
          exit(EXIT_FAILURE);
     }
     
     Node n = L->back;
     
     if (L->length == 1)
     {
           L->front = NULL;
           L->back = NULL;
           L->cursor = NULL;
           L->index = -1;
     }
     
     else
     {
          L->back = L->back->prev;
          L->back->next = NULL;
          
          if (L->index == (L->length - 1))
          {
              L->cursor = NULL;
              L->index = -1;
          }        
     }
     
     // actually deleting the node
     freeNode(&n);
     
     // adjusting the length    
     L->length -= 1;
}

void delete(List L)
{
     if (L == NULL || L->length < 1 || L->index < 0) 
     {
          fprintf(stderr, "List Error: can't delete cursor that's undefined or from an empty list\n");
          exit(EXIT_FAILURE);
     }
     
     // deleting the front 
     if (L->index == 0)
     {
          deleteFront(L);    
     }
     
     // deleting the back 
     else if (L->index == (L->length - 1))
     {
          deleteBack(L);
     }
     
     // deleting any element not in front or back
     else 
     {
          Node n = L->cursor;
          n->prev->next = n->next;
          n->next->prev = n->prev;
          
          // isolating the node from the list
          n->next = NULL;
          n->prev = NULL;
          
          L->cursor = NULL;
          L->index -= 1;
          L->length -= 1;
          
          freeNode(&n);
     }      
}


// Other operations

void printList(FILE* out, List L)
{
     Node n = L->front;
     
     // printing the 1st element 
     if (n != NULL)
     {
          fprintf(out, "%d", n->data);
          n = n->next;
     }
     
     // printing the rest of the elements
     while (n != NULL)
     {
         fprintf(out, " %d", n->data);
         n = n->next;
     }
}

List copyList(List L)
{
     if (L == NULL)
     {
          return NULL;
     }
     
     List L1 = newList();
     
     Node n = L->front;
     while (n != NULL)
     {
         append(L1, n->data);
         n = n->next;
     }
     
     return L1;
}

List concatList(List A, List B)
{
     if (A == NULL && B == NULL)
     {
          return NULL;
     }
     
     List C = newList();
     
     if (A != NULL)
     {
          Node n = A->front;
          while (n != NULL)
          {
               append(C, n->data);
               n = n->next;
          }
      }
      
     if (B != NULL)
     {
          Node n = B->front;
          while (n != NULL)
          {
               append(C, n->data);
               n = n->next;
          }
      }
      
      return C;       
}





 
