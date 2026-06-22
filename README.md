# DB Connection Pool Simulator

A minimal, multithreaded **database connection pool** implemented in C++ using raw POSIX pthreads. Demonstrates the classic producer-consumer pattern — a fixed pool of reusable connections shared safely across concurrent client threads.

---

## What It Does

- Allocates a fixed-size pool of database connections at startup
- Spawns N client threads that each **acquire** a connection, execute a query, then **release** it back
- Blocks callers cleanly when the pool is exhausted (no busy-wait, no dropped requests)
- Shuts down gracefully once all clients finish

---

## Project Structure

```
.
├── connection.h / connection.cpp       # Single connection (open, execute, get_id)
├── connection_pool.h / connection_pool.cpp  # Thread-safe pool (init, acquire, release, shutdown)
├── main.cpp                            # Driver: spawns client threads, exercises the pool
└── Makefile                            # Build with g++ -pthread
```

---

## How It Works

### Connection Pool Internals

| Component | Role |
|-----------|------|
| `connections[MAX_CONNECTIONS]` | Fixed array of pre-allocated `Connection` objects |
| `available[MAX_CONNECTIONS]` | Stack of indices pointing to free connections |
| `top` | Stack pointer — `0` means pool is empty |
| `pthread_mutex_t lock` | Serialises access to the stack |
| `pthread_cond_t not_empty` | Blocks `acquire()` callers when the pool is drained |

**Acquire** locks the mutex, waits on `not_empty` if `top == 0` (uses `while` not `if` — guards against spurious wakeups), then pops and returns the top connection.

**Release** locks the mutex, pushes the connection index back, signals `not_empty` to unblock one waiting thread.

### Simulated Latency

| Operation | Simulated delay |
|-----------|----------------|
| `Connection::open()` | 100 ms (simulates TCP + auth handshake) |
| `Connection::execute()` | 50 ms (simulates query round-trip) |

---

## Build & Run

### Requirements

| Tool | Minimum version |
|------|----------------|
| `g++` | 7+ (C++11 or later) |
| `make` | Any GNU make |
| POSIX threads (`pthread`) | Provided by glibc / macOS libc |

> **Recommended platform: Linux (Ubuntu 20.04+) or macOS.**
> On Windows use **WSL2** (Ubuntu) for the best experience — native Windows lacks POSIX thread APIs.

### Build

```bash
make
```

Produces the `pool` executable.

### Run

```bash
./pool [pool_size] [num_clients]
```

| Argument | Default | Range | Description |
|----------|---------|-------|-------------|
| `pool_size` | `4` | `1 – 16` | Number of pre-opened connections in the pool |
| `num_clients` | `10` | `1 – 256` | Number of concurrent client threads |

### Examples

```bash
# Default: 4 connections, 10 clients
./pool

# 2 connections shared by 8 clients (heavy contention)
./pool 2 8

# 8 connections, 8 clients (no waiting)
./pool 8 8

# Maximum stress: 1 connection, 20 clients
./pool 1 20
```

### Clean

```bash
make clean
```

---

## Recommended Specs for Best Results

| What | Recommendation | Why |
|------|----------------|-----|
| **OS** | Linux (Ubuntu 22.04+) or macOS 13+ | Native POSIX pthreads; no translation layer |
| **CPU** | 2+ cores | Multiple threads run truly in parallel |
| **Compiler** | `g++ 11+` or `clang++ 13+` | Better thread-sanitiser support |
| **pool_size** | Less than `num_clients` | Forces contention — shows the blocking/wakeup logic |
| **Debug build** | `g++ -Wall -pthread -fsanitize=thread` | ThreadSanitizer catches data races |

To build with ThreadSanitizer for verification:

```bash
g++ -Wall -pthread -fsanitize=thread -o pool main.cpp connection.cpp connection_pool.cpp
./pool 2 12
```