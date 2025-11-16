/***
* Raunak Anwar
* ranwar3
* 2025 Winter CSE101 PA3  
* GraphTest.c
* Source File that tests out the graph functions
***/ 

-----------------------------------------------------------------------------
//
// GraphClient.c
// 
// This is a fancy test client that calculates the Radius and Diameter of 
// the graph defined on lines 41-49, along with its Central and Peripheral 
// vertices.  The definitions of these terms at:
// 
//    http://en.wikipedia.org/wiki/Distance_(graph_theory)
// 
// Place this file in a directory with copies of your List.c, List.h,Graph.c, 
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

#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../pa3/Graph.h"
#include "../../pa3/List.h"

#define FIRST_TEST DG_getSize
#define MAXSCORE 40
#define CHARITY 10

#define RED "\033[0;31m"
#define CYAN "\033[0;36m"
#define GREEN "\033[0;32m"
#define NC "\033[0m"

static uint8_t testsPassed;
static volatile sig_atomic_t testStatus;
static uint8_t disable_exit_handler;
jmp_buf test_crash;

enum Test_e {
  DG_getSize = 0, // directed graph tests
  DG_getParent,
  DG_getDiscover,
  DG_getFinish,

  UG_getSize, // undirected graph tests
  UG_getParent,
  UG_getDiscover,
  UG_getFinish,

  NUM_TESTS,
};

char *testName(int test) {

  if (test == DG_getSize)
    return "DG_getSize";
  if (test == DG_getParent)
    return "DG_getParent";
  if (test == DG_getDiscover)
    return "DG_getDiscover";
  if (test == DG_getFinish)
    return "DG_getFinish";

  if (test == UG_getSize)
    return "UG_getSize";
  if (test == UG_getParent)
    return "UG_getParent";
  if (test == UG_getDiscover)
    return "UG_getDiscover";
  if (test == UG_getFinish)
    return "UG_getFinish";

  return "";
}

// return 0 if pass otherwise the number of the test that was failed
uint8_t runTest(Graph *pA, List *pL, int test) {
  Graph A = *pA;
  List L = *pL;
  
  // testing for getOrder
  int order = getOrder(A);
  printf("The order of the graph is %d\n", order);  
  
  // testing for transpose
  Graph A1 = newGraph(order);
  fprintf(stdout, "This is the A1 Graph\n");
  printGraph(stdout, A1);
  Graph A2 = transpose(A1);
  fprintf(stdout, "This is the transpose of A1 (A2) Graph\n");
  printGraph(stdout, A2);
  
  // Copies the adjacency list representation of A into A3
  Graph A3 = copyGraph(A); 
  printGraph(stdout, A3);
  
  switch (test) {

  case DG_getSize: {
    if (getSize(A) != 0)
      return 1;
    addArc(A, 54, 1);
    addArc(A, 54, 2);
    addArc(A, 54, 2);
    addArc(A, 54, 3);
    addArc(A, 1, 54);
    addArc(A, 1, 54);
    addArc(A, 1, 55);
    addArc(A, 1, 55);
    if (getSize(A) != 5)
      return 2;
    for (uint8_t i = 1; i <= 100; i++) {
      append(L, i);
    }
    DFS(A, L);
    if (getSize(A) != 5)
      return 3;
    addArc(A, 55, 1);
    if (getSize(A) != 6)
      return 4;
    return 0;
  }
  case DG_getParent: {
    for (uint8_t i = 1; i <= 100; i++)
      if (getParent(A, i) != NIL)
        return 1;
    addArc(A, 64, 4);
    addArc(A, 64, 3);
    addArc(A, 42, 2);
    addArc(A, 2, 64);
    addArc(A, 4, 2);
    addArc(A, 3, 42);
    for (uint8_t i = 1; i <= 100; i++) {
      append(L, i);
    }
    DFS(A, L);
    if (getParent(A, 100) != NIL)
      return 2;
    if (getParent(A, 2) != NIL)
      return 3;
    if (getParent(A, 42) != 3)
      return 4;
    if (getParent(A, 3) != 64)
      return 5;
    if (getParent(A, 4) != 64)
      return 6;
    return 0;
  }
  case DG_getDiscover: {
    for (uint8_t i = 1; i <= 100; i++)
      if (getDiscover(A, i) != UNDEF)
        return 1;
    addArc(A, 64, 4);
    addArc(A, 64, 3);
    addArc(A, 42, 2);
    addArc(A, 2, 64);
    addArc(A, 4, 2);
    addArc(A, 3, 42);
    for (uint8_t i = 1; i <= 100; i++) {
      prepend(L, i);
    }
    DFS(A, L);
    if (getDiscover(A, 100) != 1)
      return 2;
    if (getDiscover(A, 64) != 73)
      return 3;
    if (getDiscover(A, 4) != 80)
      return 4;
    DFS(A, L);
    if (getDiscover(A, 4) != 126)
      return 5;
    if (getDiscover(A, 63) != 117)
      return 6;
    DFS(A, L);
    if (getDiscover(A, 65) != 71)
      return 7;
    if (getDiscover(A, 1) != 199)
      return 8;
    return 0;
  }
  case DG_getFinish: {
    for (uint8_t i = 1; i <= 100; i++)
      if (getFinish(A, i) != UNDEF)
        return 1;
    addArc(A, 64, 4);
    addArc(A, 64, 3);
    addArc(A, 42, 2);
    addArc(A, 2, 64);
    addArc(A, 4, 2);
    addArc(A, 3, 42);
    for (uint8_t i = 1; i <= 100; i++) {
      prepend(L, i);
    }
    DFS(A, L);
    if (getFinish(A, 100) != 2)
      return 2;
    if (getFinish(A, 64) != 82)
      return 3;
    if (getFinish(A, 42) != 78)
      return 4;
    DFS(A, L);
    if (getFinish(A, 64) != 128)
      return 5;
    if (getFinish(A, 63) != 118)
      return 6;
    DFS(A, L);
    if (getFinish(A, 65) != 72)
      return 7;
    if (getFinish(A, 1) != 200)
      return 8;
    return 0;
  }
  case UG_getSize: {
    if (getSize(A) != 0)
      return 1;
    addEdge(A, 54, 1);
    addEdge(A, 54, 2);
    addEdge(A, 54, 3);
    addEdge(A, 1, 55);
    if (getSize(A) != 4)
      return 2;
    for (uint8_t i = 1; i <= 100; i++) {
      append(L, i);
    }
    DFS(A, L);
    if (getSize(A) != 4)
      return 3;
    addEdge(A, 1, 56);
    if (getSize(A) != 5)
      return 4;
    return 0;
  }
  case UG_getParent: {
    for (uint8_t i = 1; i <= 100; i++)
      if (getParent(A, i) != NIL)
        return 1;
    addEdge(A, 64, 4);
    addEdge(A, 64, 3);
    addEdge(A, 42, 2);
    addEdge(A, 2, 64);
    addEdge(A, 4, 2);
    addEdge(A, 3, 42);
    for (uint8_t i = 1; i <= 100; i++) {
      append(L, i);
    }
    DFS(A, L);
    if (getParent(A, 100) != NIL)
      return 2;
    if (getParent(A, 2) != NIL)
      return 3;
    if (getParent(A, 42) != 3)
      return 4;
    if (getParent(A, 3) != 64)
      return 5;
    if (getParent(A, 4) != 2)
      return 6;
    return 0;
  }
  case UG_getDiscover: {
    for (uint8_t i = 1; i <= 100; i++)
      if (getDiscover(A, i) != UNDEF)
        return 1;
    addEdge(A, 64, 4);
    addEdge(A, 64, 3);
    addEdge(A, 42, 2);
    addEdge(A, 2, 64);
    addEdge(A, 4, 2);
    addEdge(A, 3, 42);
    for (uint8_t i = 1; i <= 100; i++) {
      prepend(L, i);
    }
    DFS(A, L);
    if (getDiscover(A, 100) != 1)
      return 2;
    if (getDiscover(A, 64) != 73)
      return 3;
    if (getDiscover(A, 4) != 75)
      return 4;
    DFS(A, L);
    if (getDiscover(A, 4) != 121)
      return 5;
    if (getDiscover(A, 63) != 117)
      return 6;
    DFS(A, L);
    if (getDiscover(A, 65) != 71)
      return 7;
    if (getDiscover(A, 1) != 199)
      return 8;
    return 0;
  }
  case UG_getFinish: {
    for (uint8_t i = 1; i <= 100; i++)
      if (getFinish(A, i) != UNDEF)
        return 1;
    addEdge(A, 64, 4);
    addEdge(A, 64, 3);
    addEdge(A, 42, 2);
    addEdge(A, 2, 64);
    addEdge(A, 4, 2);
    addEdge(A, 3, 42);
    for (uint8_t i = 1; i <= 100; i++) {
      prepend(L, i);
    }
    DFS(A, L);
    if (getFinish(A, 100) != 2)
      return 2;
    if (getFinish(A, 64) != 82)
      return 3;
    if (getFinish(A, 42) != 80)
      return 4;
    DFS(A, L);
    if (getFinish(A, 64) != 128)
      return 5;
    if (getFinish(A, 63) != 118)
      return 6;
    DFS(A, L);
    if (getFinish(A, 65) != 72)
      return 7;
    if (getFinish(A, 1) != 200)
      return 8;
    return 0;
  }
  }
  
  // Print adjacency list representation of G
   printGraph(stdout, G);
   
  return 255;
}

void segfault_handler(int signal) { // everyone knows what this is
  testStatus = 255;
  longjmp(test_crash, 1);
}

void exit_attempt_handler(void) { // only I decide when you are done
  if (disable_exit_handler)
    return; // allow this to be disabled
  testStatus = 255;
  longjmp(test_crash, 2);
}

void abrupt_termination_handler(int signal) { // program killed externally
  testStatus = 255;
  longjmp(test_crash, 3);
}

int main(int argc, char **argv) {
  if (argc > 2 || (argc == 2 && strcmp(argv[1], "-v") != 0)) {
    printf("Usage: %s [-v]", (argc > 0 ? argv[0] : "./GraphTest"));
    exit(1);
  }

  printf("\n"); // more spacing
  if (argc == 2)
    printf("\n"); // consistency in verbose mode

  testsPassed = 0;
  disable_exit_handler = 0;
  atexit(exit_attempt_handler);
  signal(SIGSEGV, segfault_handler);

  for (uint8_t i = FIRST_TEST; i < NUM_TESTS; i++) {
    Graph A = newGraph(100);
    List L = newList();
    uint8_t fail_type = setjmp(test_crash);
    if (fail_type == 0) {
      testStatus = runTest(&A, &L, i);
      freeGraph(&A);
      freeList(&L);
    }
    if (argc == 2) { // it's verbose mode
      printf("Test %s: %s", testName(i),
             testStatus == 0 ? GREEN "PASSED" NC : RED "FAILED" NC);
      if (testStatus == 255) {
        printf(": due to a %s\n", fail_type == 1   ? "segfault"
                                  : fail_type == 2 ? "program exit"
                                                   : "program interruption");
        printf(RED "\nWARNING: Program will now stop running tests\n\n" NC);
        break;

      } else if (testStatus == 254) {
        printf(": undefined test\n");
      } else if (testStatus != 0) {
        printf(": test %d\n", testStatus);
      } else {
        printf("\n");
      }
    }
    if (testStatus == 0) {
      testsPassed++;
    }
  }

  disable_exit_handler = 1;

  uint8_t totalScore = (MAXSCORE - NUM_TESTS * 5) + testsPassed * 5;

  if (argc == 2 && testStatus != 255)
    printf("\nYou passed %d out of %d tests\n", testsPassed, NUM_TESTS);
  else if (testStatus == 255) {
    totalScore = CHARITY; // charity points
    if (argc == 2)
      printf(RED "Receiving charity points because your program crashes\n" NC);
  }
  printf("\nYou will receive %d out of %d possible points on the GraphTest\n\n",
         totalScore, MAXSCORE);

  return 0;
}


