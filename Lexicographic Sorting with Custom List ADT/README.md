# Lexicographic Sorter Using a Custom Doubly-Linked List (C Project)

This project implements a complete **List Abstract Data Type (ADT)** in C using a custom doubly-linked list with cursor-based navigation. The List ADT is then used by **Lex.c**, a lexicographic sorting program that reads all lines from an input file, stores their indices in a List, sorts them in lexicographical order, and writes the sorted output to a file.

The project demonstrates low-level memory management, ADT design, modular programming, and pointer manipulation — core concepts in systems-level C development.

---

## 🔧 Project Overview
- Fully custom **doubly-linked list** with:
  - Cursor movement  
  - Prepend/append  
  - insertBefore / insertAfter  
  - delete and clear operations  
  - copy and concatenate functionality  
- No external libraries — everything manually implemented.
- `Lex.c` uses the List ADT to perform **index-based lexicographic sorting** of strings.
- Independent test suite ensures correctness of all ADT operations.

---

## 📁 Files Included

### **List.h**
Defines the List ADT interface, including:
- Struct definitions  
- Function prototypes  
- Cursor behavior  
- Constants and macros  

### **List.c**
Implements all List ADT functionality:
- Node management  
- Cursor tracking  
- All insert, delete, and navigation operations  
- Proper memory allocation and cleanup  

### **ListTest.c**
Test harness for validating List ADT behavior:
- Tests every List operation  
- Includes edge cases (empty list, single element, cursor at boundaries)

### **Lex.c**
Client program that:
- Reads input file lines  
- Stores line indices in the List  
- Sorts them lexicographically using insertion ordering  
- Writes sorted results to an output file  
- Performs command-line argument and error checking  

### **Makefile**
Automates compilation:
- Builds all modules  
- Links the final executable  
- Provides `make`, `make clean`, and other useful targets  

---

## 🧠 Skills Demonstrated
- Custom data structure design (ADT)
- Deep pointer manipulation and dynamic memory handling
- Cursor-based linked list operations  
- File I/O and string processing  
- Modular multi-file C software architecture  
- Test-driven development (TDD approach)
- Makefile-based compilation for multi-module projects

---

## ▶️ Build & Run
```bash
make
./Lex input.txt output.txt
