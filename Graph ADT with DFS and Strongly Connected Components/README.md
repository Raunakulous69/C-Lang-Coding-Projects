# Graph ADT with DFS and Strongly Connected Components (C Project)

This project implements a full **Graph Abstract Data Type (ADT)** and custom **List ADT** in C to perform **Depth-First Search (DFS)** and compute **Strongly Connected Components (SCCs)** in directed graphs. Using adjacency lists and the classical two-pass DFS algorithm (Kosaraju–Sharir), the program identifies and prints all SCCs in the correct component order.

The codebase emphasizes modular ADT design, pointer-based dynamic memory management, and graph-theoretic algorithm implementation.

---

## 📁 Files Included

### **List.h**
Header file defining the public interface for the List ADT.  
Includes type definitions, exported operations, and cursor-based list behavior.

### **List.c**
Implementation of the custom cursor-based doubly linked List ADT.  
Supports insertion, deletion, traversal, copying, and concatenation operations used by DFS.

---

### **Graph.h**
Header file defining the Graph ADT interface.  
Declares all graph operations including constructors, addArc/addEdge, DFS accessors, transpose, and symbolic constants (`NIL`, `UNDEF`).

### **Graph.c**
Implementation of the Graph ADT using adjacency lists.  
Includes:
- Graph construction and deletion  
- `addArc()` and `addEdge()`  
- Full DFS algorithm with discover/finish times  
- Transpose and copyGraph operations  
- Parent and time-stamp tracking

---

### **GraphTest.c**
Standalone tester verifying Graph ADT behavior.  
Checks:
- Vertex properties  
- addEdge/addArc correctness  
- DFS discover/finish behavior  
- Transpose correctness  
- Edge-case handling (disconnected graphs, cycles, etc.)

---

### **FindComponents.c**
Main program that:
1. Reads a directed graph from an input file  
2. Runs DFS on the original graph  
3. Runs DFS on the transpose graph using descending finish-time order  
4. Identifies all **strongly connected components**  
5. Writes adjacency lists and SCCs to an output file  

Demonstrates practical application of DFS and the Graph ADT to solve a real graph-processing problem.

---

### **Makefile**
Automates compilation and linking for all modules.  
Supports:
```bash
make
make clean
