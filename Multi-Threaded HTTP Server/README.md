# 🚀 Multi-Threaded HTTP Server (C)

A high-performance multi-threaded HTTP/1.1 server written in C that supports concurrent client requests using a thread pool architecture. This project combines networking, synchronization, and concurrent data structures to deliver scalable and efficient request handling with strong consistency guarantees.

---

## 🚀 Features

- 🌐 Handles HTTP/1.1 client-server communication  
- ⚡ Multi-threaded request processing with thread pool  
- 🔄 Supports concurrent GET and PUT operations  
- 🧵 Thread-safe request queue and synchronization  
- 📜 Audit logging with strict ordering guarantees  
- 🛡️ No memory leaks, race conditions, or crashes  

---

## ⚙️ How It Works

The server accepts client connections and distributes them across worker threads.

### Run Server
```
./httpserver [-t threads] <port>
```

- `<port>`: required (1–65535)  
- `threads`: optional (default = 4)  

---

## 🔄 Architecture

### Thread Pool Design

- **Dispatcher Thread**
  - Accepts incoming client connections  
  - Pushes requests into a thread-safe queue  

- **Worker Threads (N)**
  - Pop requests from queue  
  - Process HTTP requests (GET / PUT)  
  - Send responses back to clients  

---

## 📡 Supported Methods

### GET
- Retrieves file content  
- Returns:
  - `200 OK` if file exists  
  - `404 Not Found` if file does not exist  

---

### PUT
- Creates or overwrites file  
- Returns:
  - `201 Created` for new file  
  - `200 OK` for overwrite  

---

## 📜 Audit Log

Each request is logged to `stderr` in the format:
```
<Operation>,<URI>,<Status-Code>,<RequestID>
```

Example:
```
GET,/a.txt,200,1
PUT,/b.txt,201,3
```

### Guarantees
- Atomic log entries (no corruption)  
- Durable logging  
- Defines total ordering (linearization) of requests  

---

## 🔐 Concurrency & Synchronization

- Thread-safe queue for request dispatching  
- Reader-writer locks for file access coordination  
- Ensures:
  - Atomic request processing  
  - Coherent ordering across threads  
  - No race conditions or data corruption  

---

## 📊 Performance

- Improves **throughput** via parallel request handling  
- Minimizes blocking when requests are independent  
- Synchronizes only when necessary (shared resources)  

---

## 🛠️ Tech Stack

- **Language:** C  
- **Libraries:** pthreads (threads, mutexes, condition variables)  
- **Concepts:**  
  - Multi-threading & thread pools  
  - Producer-consumer model  
  - Reader-writer locks  
  - Socket programming  
  - HTTP protocol  

---

## 📂 Project Structure

```
.
├── httpserver.c      # Multi-threaded server implementation
├── Makefile          # Build configuration
├── README.md         # Project documentation
├── asgn4_helper_funcs.a  # Provided helper library
```

---

## 🧪 Build & Run

### Compile
```
make
```

### Run
```
./httpserver -t 4 1234
```

---

## 🧪 Testing

### Using curl
```
curl http://localhost:1234/file.txt
curl http://localhost:1234/new.txt -T file.txt
```

### Advanced Testing
- Custom concurrency test scripts  
- Validated audit logs and request ordering  

---

## 📊 Constraints

- Uses ≤ 10 MB memory  
- No busy waiting  
- No file descriptor leaks  
- No crashes under malformed or concurrent requests  

---

## 🎯 Key Takeaways

- Built a scalable multi-threaded server from scratch  
- Implemented synchronization for correctness under concurrency  
- Designed a system that balances performance with consistency  
- Gained deep understanding of thread coordination and system design  

---

## 📌 Notes

- Written entirely in C (no external program execution)  
- Uses pthreads for concurrency  
- Designed for Linux environments  
