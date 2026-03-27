# 🌐 HTTP Server (C)

A lightweight HTTP/1.1 server written in C that handles client-server communication using sockets. This project demonstrates systems-level programming, including network communication, request parsing, and efficient file I/O, while implementing core HTTP methods (GET and PUT).

---

## 🚀 Features

- 🌍 Handles HTTP/1.1 client-server communication  
- 📥 GET – Retrieves file contents from server  
- 📤 PUT – Creates or updates files on server  
- ⚡ Efficient buffered I/O using low-level system calls  
- 🛡️ Robust error handling for malformed requests  
- 🔒 No memory leaks or file descriptor leaks  

---

## ⚙️ How It Works

The server runs continuously and processes client connections sequentially.

### Run Server
```
./httpserver <port>
```

- `<port>` must be between 1 and 65535  
- Recommended: use ports ≥ 1023  

---

## 🔄 Server Workflow

1. Create and listen on a socket  
2. Accept incoming client connection  
3. Parse HTTP request  
4. Execute request logic (GET / PUT)  
5. Send HTTP response  
6. Close connection  

---

## 📡 Supported HTTP Methods

### 1. GET
- Retrieves file content from server  
- Returns:
  - `200 OK` if file exists  
  - `404 Not Found` if file does not exist  

---

### 2. PUT
- Creates or overwrites a file  
- Returns:
  - `201 Created` if new file created  
  - `200 OK` if file overwritten  

---

## 📥 Example Requests

### GET Request
```
GET /foo.txt HTTP/1.1\r\n\r\n
```

### PUT Request
```
PUT /foo.txt HTTP/1.1\r\nContent-Length: 12\r\n\r\nHello World!
```

---

## 📤 Example Response

```
HTTP/1.1 200 OK\r\n
Content-Length: 12\r\n
\r\n
Hello World!
```

---

## ❌ Error Handling

The server returns appropriate HTTP status codes:

- `400 Bad Request` – malformed request  
- `403 Forbidden` – file access denied  
- `404 Not Found` – file does not exist  
- `500 Internal Server Error` – unexpected failure  
- `501 Not Implemented` – unsupported method  
- `505 Version Not Supported` – invalid HTTP version  

---

## 🛠️ Tech Stack

- **Language:** C  
- **Concepts:**  
  - Socket programming  
  - Client-server architecture  
  - HTTP protocol parsing  
  - Memory management  
  - File I/O (read/write system calls)  

---

## 📂 Project Structure

```
.
├── httpserver.c      # Core server implementation
├── Makefile          # Build configuration
├── README.md         # Project documentation
├── asgn2_helper_funcs.a  # Provided helper library
```

---

## 🧪 Build & Run

### Compile
```
make
```

### Run Server
```
./httpserver 1234
```

---

## 🧪 Testing

### Using curl
```
curl http://localhost:1234/foo.txt
curl http://localhost:1234/new.txt -T foo.txt
```

### Using netcat
```
printf "GET /foo.txt HTTP/1.1\r\n\r\n" | nc localhost 1234
```

---

## 📊 Performance & Constraints

- Uses ≤ 10 MB memory  
- Handles multiple connections sequentially  
- Efficient request parsing and file transfer  
- No crashes on malformed input  

---

## 🎯 Key Takeaways

- Built a fully functional HTTP server from scratch  
- Implemented networking with sockets and low-level system calls  
- Developed strong understanding of HTTP protocol and request handling  
- Designed a resilient system that handles real-world edge cases  

---

## 📌 Notes

- Written entirely in C (no external programs used)  
- Avoids restricted stdio functions (`fread`, `fwrite`, etc.)  
- Designed for Linux environments  
