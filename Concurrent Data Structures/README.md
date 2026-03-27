# ⚙️ Concurrent Data Structures (C)

A multi-threaded systems programming project in C implementing a **thread-safe bounded buffer (queue)** and a **reader-writer lock** with configurable priority policies. This project demonstrates core concepts in concurrency, synchronization, and parallel systems design.

---

## 🚀 Features

- 🔄 Thread-safe **bounded FIFO queue** (producer-consumer model)  
- 🔐 Custom **reader-writer lock** with priority control  
- ⚡ Supports concurrent readers and writers efficiently  
- 🛡️ Guarantees no data races, deadlocks, or memory leaks  
- 🎯 Implements fairness policies (Readers, Writers, N-Way)  

---

## ⚙️ Components

### 1. Bounded Buffer (Queue)

Implements a thread-safe FIFO queue storing generic pointers.

#### API
```
queue_t *queue_new(int size);
void queue_delete(queue_t **q);
bool queue_push(queue_t *q, void *elem);
bool queue_pop(queue_t *q, void **elem);
```

#### Behavior
- Blocks on:
  - `push` when queue is full  
  - `pop` when queue is empty  
- Guarantees:
  - FIFO ordering  
  - No lost or duplicated data  
  - Safe concurrent producers and consumers  

---

### 2. Reader-Writer Lock

Allows concurrent reads and exclusive writes with configurable priority.

#### API
```
rwlock_t *rwlock_new(PRIORITY p, int n);
void rwlock_delete(rwlock_t **l);
void reader_lock(rwlock_t *rw);
void reader_unlock(rwlock_t *rw);
void writer_lock(rwlock_t *rw);
void writer_unlock(rwlock_t *rw);
```

#### Priority Modes
- `READERS` – prioritize readers  
- `WRITERS` – prioritize writers  
- `N_WAY` – allow `n` readers between writers  

#### Guarantees
- Multiple readers allowed simultaneously  
- Only one writer at a time  
- Prevents starvation for both readers and writers  
- Maintains fairness under contention  

---

## 🛠️ Tech Stack

- **Language:** C  
- **Libraries:** pthreads (mutexes, condition variables)  
- **Concepts:**  
  - Concurrency & synchronization  
  - Producer-consumer pattern  
  - Reader-writer problem  
  - Thread safety & race condition prevention  

---

## 📂 Project Structure

```
.
├── queue.c        # Thread-safe bounded buffer implementation
├── rwlock.c       # Reader-writer lock implementation
├── Makefile       # Build configuration
├── README.md      # Project documentation
```

---

## 🧪 Build

```
make
```

Produces:
- `queue.o`
- `rwlock.o`

---

## 🧪 Testing

- Local autograder scripts  
- Custom multi-threaded tests  
- Verified for:
  - Thread safety  
  - Correct ordering (FIFO)  
  - No deadlocks or starvation  

---

## 📊 Performance & Constraints

- No busy-waiting (uses condition variables)  
- Efficient thread synchronization  
- No memory leaks or dangling pointers  
- Handles high concurrency reliably  

---

## 🎯 Key Takeaways

- Built concurrent data structures from scratch  
- Implemented synchronization using low-level primitives  
- Designed systems that guarantee correctness under parallel execution  
- Gained deep understanding of race conditions, fairness, and thread coordination  

---

## 📌 Notes

- Written entirely in C (no C++ or external libraries)  
- Designed for Linux environments  
- Uses `clang` with strict flags (`-Wall -Wextra -Werror -pedantic`)  
