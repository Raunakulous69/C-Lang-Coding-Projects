# Graph ADT with Breadth-First Search & Shortest Path Reconstruction (C Project)

This project implements a full **Graph Abstract Data Type (ADT)** along with a custom **List ADT**, and uses them to perform **Breadth-First Search (BFS)** and compute **shortest paths** between vertices. The project consists of a modular C implementation split into separate files for the List ADT, Graph ADT, test programs, and a client program (`FindPath`) that reads graph data from an input file and outputs adjacency lists and shortest paths.

This repository demonstrates graph algorithms, ADT design, dynamic memory management, and structured C programming across multiple modules.

---

## 📁 Files Included

### **List.h**  
Header file defining the interface for the custom doubly-linked List ADT, including:
- Struct definitions  
- Cursor movement operations  
- Insert/delete functions  
- Copying and concatenation helpers  
- Printing utilities  
:contentReference[oaicite:0]{index=0}

---

### **List.c**  
Implementation of the **cursor-based doubly-linked list**, including:
- Node creation & deletion  
- Cursor tracking  
- Prepend, append, insertBefore, insertAfter  
- deleteFront / deleteBack / delete  
- copyList and concatList  
- printList for formatted output  
:contentReference[oaicite:1]{index=1}

This List ADT is used internally by the Graph ADT for adjacency lists and BFS queue operations.

---

### **Graph.h**  
Header file defining the **Graph ADT interface**, including:
- Graph constructor/destructor  
- Access functions (`getOrder`, `getSize`, `getSource`, `getParent`, `getDist`)  
- BFS function prototype  
- `INF` and `NIL` symbolic constants  
- `printGraph` formatting specification  
:contentReference[oaicite:2]{index=2}

---

### **Graph.c**  
Full implementation of the **Graph ADT**, using an array of List ADTs for adjacency lists. Includes:
- Graph initialization using arrays of Lists  
- `addEdge` and `addArc` with sorted insertion  
- BFS implementation using queue-style List operations  
- Distance, parent, and color tracking  
- Shortest-path reconstruction through parent pointers via `getPath`  
- `printGraph` for adjacency list output  
:contentReference[oaicite:3]{index=3}

---

### **GraphTest.c**  
A standalone tester that:
- Builds a graph with 35 vertices  
- Computes eccentricities of all vertices  
- Calculates **Radius**, **Diameter**, **Central vertices**, and **Peripheral vertices**  
- Tests BFS, access functions, path reconstruction, and makeNull  
:contentReference[oaicite:4]{index=4}

This file ensures correctness of BFS, adjacency management, and path logic.

---

### **FindPath.c**  
Main client program. It:
1. Reads graph data from an input file  
2. Builds the graph with `addEdge`  
3. Prints the adjacency list  
4. Reads (source, destination) queries  
5. Runs BFS for each query  
6. Prints either the distance and a shortest path, or states “No path exists”  
:contentReference[oaicite:5]{index=5}

This file demonstrates practical use of the Graph ADT in solving shortest-path problems using BFS.

---

### **Makefile**  
Automates compilation, linking, cleanup, and executable creation for all modules.  
:contentReference[oaicite:6]{index=6}

---

## 🧠 Concepts Demonstrated
- Abstract Data Types (ADTs) in C  
- Doubly-linked list implementation with cursor operations  
- Graph representation using adjacency lists  
- Breadth-First Search (BFS) for unweighted shortest paths  
- Path reconstruction using parent pointers  
- Dynamic memory allocation and cleanup  
- Input parsing and formatted output  
- Modular multi-file C system with appropriate headers  

---

## ▶️ Build & Run

To compile everything:
```bash
make
