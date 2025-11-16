/***
* Raunak Anwar
* ranwar3
* 2025 Winter CSE101 PA1  
* Lex.c
* Main Source File (Client Program) 
***/ 
#define _POSIX_C_SOURCE 200809L 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"

void usage(char *exc)
{
    fprintf(stderr,
         "SYNOPSIS:\n"
         "  Read the content of a given text file and save them in\n"
         "  lexographical order to the given output file\n"
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
     
     // coumt the number of lines in the input file
     int lineCount = 0;
     
     int c = 0;
     
     int lastChar = 0;
     
     while ((c = getc(in)) != EOF)
     {
          if (c == '\n')
          {
               lineCount++;
          }
          
          lastChar = c;
     }
     
     if (lastChar != '\n')
     {
         lineCount++;
     }
     
     // If the input file is empty, then we do not need to do anything
     if (lineCount == 0)
     {
         fclose(in);
         fclose(out);
         return 0;
     }
     
     // allocating memory for the lines in input file
     char** arr = calloc(lineCount, sizeof(char*));
     
     // reading each line from the file and add them to the array
     size_t lineSize = 0;
     
     // rewinding to the file pointer to the start
     fseek(in, SEEK_SET, 0);
     
     for (int i = 0; i < lineCount; i++)
     {
          getline(&(arr[i]), &lineSize, in);
     }
     
     // creating a list to hold the incdices of the array in lexographical 
     // order
     List L = newList();
     append(L,0);
     
     for (int i = 1; i < lineCount; i++)
     {
          moveBack(L);
          int j = index(L);
          
          while (j >= 0 && strcmp(arr[get(L)], arr[i]) > 0)
          {
               movePrev(L);
               j = index(L);
          }
          
          if (j < 0)
          {
               prepend(L, i);
          }
          
          else 
          {
               insertAfter(L, i);
          }
     }
     
     // printing the lines into the given file
     moveFront(L);
     
     while (index(L) >= 0)
     {
          fputs(arr[get(L)], out);
          moveNext(L);
     }
     
     // free the allocated memory
     for (int i = 0; i < lineCount; i++)
     {
          free(arr[i]);
          arr[i] = NULL;
     }
     
     free(arr);
     arr = NULL;
     freeList(&L);
         
     // printf("Number of lines: %d\n", lineCount);

     // closing the files
     fclose(in);
     fclose(out);

     return 0;
}
