# 🧠 Command-Line Memory (C)

A lightweight Linux-based command-line program written in C that provides a simple **get/set memory abstraction for files**. This project demonstrates low-level systems programming concepts including file I/O, memory management, and string parsing using system calls.

---

## 🚀 Features

- 📥 Get Operation – Reads and outputs file contents  
- 📤 Set Operation – Creates or overwrites files with specified content  
- ⚡ Efficient buffered I/O for performance  
- 🛡️ Robust error handling for invalid commands and edge cases  
- 🔒 Safe memory and file descriptor management (no leaks)

---

## ⚙️ How It Works

The program reads commands from `stdin` and executes them in the current working directory.

### Supported Commands

#### 1. Get File Contents
```
get
<filename>
```
Outputs the contents of the file to `stdout`.

---

#### 2. Set File Contents
```
set
<filename>
<content_length>
<content>
```
Writes `<content_length>` bytes to the specified file.

---

### ❌ Invalid Commands
- Outputs: `Invalid Command`
- Exit code: `1`

---

### ⚠️ Error Handling
- Outputs: `Operation Failed`
- Handles:
  - Missing files
  - Invalid filenames
  - Partial reads/writes

---

## 🛠️ Tech Stack

- **Language:** C  
- **Concepts:**  
  - Linux system calls (`read`, `write`, `open`, `close`)  
  - Memory management  
  - File descriptor handling  
  - String parsing  

---

## 📂 Project Structure

```
.
├── memory.c        # Core implementation
├── Makefile        # Build configuration
├── README.md       # Project documentation
```

---

## 🧪 Build & Run

### Compile
```
make
```

### Run
```
./memory
```

### Example Usage
```
echo -e "get\nfoo.txt" | ./memory
```

---

## 📊 Performance & Constraints

- Uses ≤ 10 MB memory  
- Efficient buffered file operations  
- No memory leaks (validated via Valgrind)  
- Handles variable-length input robustly  

---

## 🎯 Key Takeaways

- Built a low-level file abstraction system from scratch  
- Strengthened understanding of systems programming and OS-level APIs  
- Designed a reliable and efficient tool under strict constraints  

---

## 📌 Notes

- Written entirely in C (no external program execution)  
- Avoids restricted stdio functions (e.g., `fread`, `fwrite`)  
- Designed for Linux environments  
