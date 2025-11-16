#------------------------------------------------------------------------------
# Makefile for CSE 101 Programming Assignment 2
#
# make                   makes 
# make GraphTest         makes GraphTest
# make clean             removes all binaries
# make checkClient       checks ListClient for memory errors
# make checkFindPath     checks FindPath for memory errors
#------------------------------------------------------------------------------


FindPath : FindPath.o Graph.o List.o
	gcc -std=c17 -Wall -o FindPath FindPath.o Graph.o List.o

FindPath.o : FindPath.c List.h
	gcc -std=c17 -Wall -c FindPath.c

GraphTest: GraphTest.o List.o Graph.o
	gcc -std=c17 -Wall -o GraphTest GraphTest.o List.o Graph.o

GraphTest.o : GraphTest.c List.h
	gcc -std=c17 -Wall -c GraphTest.c

List.o : List.c List.h
	gcc -std=c17 -Wall -c List.c
	

Graph.o : Graph.c Graph.h
	gcc -std=c17 -Wall -c Graph.c

clean :
	rm -f FindPath GraphTest *.o

checkClient : 
	valgrind --leak-check=full ./GraphTest

checkFindPath :
	valgrind --leak-check=full ./FindPath in3 blah3
	
