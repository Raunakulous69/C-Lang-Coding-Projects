# Matrix ADT with Sparse Matrix Operations (C Project)

This project implements a full **Matrix Abstract Data Type (ADT)** in C using a custom **List ADT** to represent sparse matrices efficiently. The program supports matrix creation, manipulation, arithmetic operations, transposition, and matrix multiplication using linked-list–based row structures. The top-level program (`Sparse.c`) reads two sparse matrices from an input file, performs a series of matrix operations, and prints the results to an output file.

The project demonstrates modular ADT design, dynamic memory management, and efficient handling of sparse numerical data structures.

---

## 📁 Files Included

### **1. List.h**
Header file defining the public interface for the List ADT.  
Provides type definitions and declarations for cursor-based operations used throughout the Matrix ADT.  
:contentReference[oaicite:0]{index=0}

### **2. List.c**
Implementation of the doubly linked List ADT with cursor support.  
Handles insertion, deletion, traversal, and concatenation — serving as the underlying structure for each row of the sparse matrix.  
:contentReference[oaicite:1]{index=1}

---

### **3. Matrix.h**
Header file defining the Matrix ADT interface, including constructors, arithmetic operations, access functions, and printing utilities.  
Specifies the sparse matrix representation and required preconditions.  
:contentReference[oaicite:2]{index=2}

### **4. Matrix.c**
Full implementation of the Matrix ADT using an array of List ADTs.  
Supports:
- `changeEntry()` to update or remove matrix elements  
- Scalar multiplication  
- Matrix addition and subtraction  
- Matrix transpose  
- Matrix product (using row/column dot products)  
- Efficient non-zero (NNZ) tracking  
:contentReference[oaicite:3]{index=3}

---

### **5. MatrixTest.c**
A standalone test client for the Matrix ADT.  
Runs correctness tests for scalar multiplication, transpose, addition, subtraction, product, NNZ counting, and equality checks.  
:contentReference[oaicite:4]{index=4}

---

### **6. ListTest.c**
A test client for validating List ADT behavior.  
Checks cursor movement, insertion, deletion, copying, concatenation, and memory correctness.  
:contentReference[oaicite:5]{index=5}

---

### **7. Sparse.c**
Top-level program that:
1. Reads matrix dimensions and non-zero entries from an input file  
2. Constructs sparse matrices **A** and **B**  
3. Prints both matrices and their NNZ counts  
4. Performs matrix operations including:  
   - `1.5 * A`  
   - `A + B`  
   - `A + A`  
   - `B - A`  
   - `A - A`  
   - `transpose(A)`  
   - `A * B`  
   - `B * B`  
5. Writes all results to an output file in formatted sparse-matrix form  

This file demonstrates the complete use of the Matrix ADT in a real application.  
:contentReference[oaicite:6]{index=6}

---

### **8. Makefile**
Automates compilation of all modules, building:
- `Sparse`  
- `MatrixTest`  
- `ListTest`  

Ensures proper linking and allows easy rebuilding or cleaning of the project.

---

## 🧠 Concepts Demonstrated
- Sparse matrix representation with dynamic row lists  
- Matrix arithmetic and algebraic operations  
- Linked-list data structures for numerical storage  
- Modular multi-file C programming  
- Pointer-based memory management  
- Input parsing and formatted output  
- Test-driven development with dedicated test clients

---

## ▶️ Build Instructions
```bash
make
