/***
* Raunak Anwar
* ranwar3
* 2025 Winter CSE101 PA1  
* ListTest.c
* Test client for List ADT
***/
/****************************************************************************************
 *  ListTest.c
 *  Testing for List ADT
 *****************************************************************************************/

#include "List.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[]) {

  List A = newList();
  List B = newList();
  List C = NULL;
  int i;

  for (i = 1; i <= 20; i++) {
    append(A, i);
    prepend(B, i);
  }

  printList(stdout, A);
  printf("\n");
  printList(stdout, B);
  printf("\n");

  for (moveFront(A); index(A) >= 0; moveNext(A)) {
    printf("%d ", get(A));
  }
  printf("\n");
  for (moveBack(B); index(B) >= 0; movePrev(B)) {
    printf("%d ", get(B));
  }
  printf("\n");

  C = copyList(A);
  printf("%s\n", equals(A, B) ? "true" : "false");
  printf("%s\n", equals(B, C) ? "true" : "false");
  printf("%s\n", equals(C, A) ? "true" : "false");

  moveFront(A);
  for (i = 0; i < 5; i++)
    moveNext(A);       // at index 5
  insertBefore(A, -1); // at index 6
  for (i = 0; i < 9; i++)
    moveNext(A); // at index 15
  insertAfter(A, -2);
  for (i = 0; i < 5; i++)
    movePrev(A); // at index 10
  delete (A);
  printList(stdout, A);
  printf("\n");
  printf("%d\n", length(A));
  clear(A);
  printf("%d\n", length(A));

  freeList(&A);
  freeList(&B);
  freeList(&C);
  
  // testing for front, back, and set
  List D = newList();
  append(D,3);
  
  assert (front(D) == 3);
  assert (back(D) == 3);
  
  moveFront(D); // setting cursor to the front element
  
  set(D,4);
  
  assert (front(D) == 4);
  assert (back(D) == 4);
  
  append(D,7);
  
  assert (front(D) == 4);
  assert (back(D) == 7);
  
  // testing for deleteFront 
  deleteFront(D);
  assert (length(D) == 1);
  assert (front(D) == 7);
  
  //testing for deleteBack
  prepend(D,4); // D = [4,7]
  assert (length(D) == 2);
  assert (back(D) == 7);
  assert (front(D) == 4);
  
  deleteBack(D);
  assert(length(D) == 1);
  assert (back(D) == 4);
  assert (front(D) == 4);
  
  // testing for concat_List
  append(D, 7); // D = [4,7]
  
  List E = newList();
  append(E, 10);
  append(E, 20);
  append(E, 30);
  
  List F = concatList(D,E); // F = [4,7,10,20,30]
  assert (F != NULL);
  assert (length(F) == 5);
  assert (front(F) == 4);
  assert (back(F) == 30);
  
  freeList(&D);
  freeList(&E);
  freeList(&F);    
 
  return (0);
}

/*
Output of this program:
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
false
false
true
1 2 3 4 5 -1 6 7 8 9 11 12 13 14 15 -2 16 17 18 19 20
21
0
*/

