# DB Connection Pool

A multithreaded connection pool in C++ using pthreads. Fixed pool of connections shared across threads — threads block when the pool is empty and wake up when one is released.

## How it works

- Pool is initialized with N connections at startup
- Client threads call `acquire()` to get a connection, `release()` when done
- `acquire()` blocks if the pool is empty (condition variable, no busy-wait)
- Connections are stored as a stack internally (array + top pointer)

Singleton pattern is used so only one pool instance exists across the program.

## Build

Needs g++ and pthreads (Linux/macOS or WSL on Windows).

```bash
make
```

## Run

```bash
./pool [pool_size] [num_clients]
```

Defaults to 4 connections, 10 clients. Example with heavy contention:

```bash
./pool 2 8
```

## Files

- `connection.h / .cpp` — single connection object
- `connection_pool.h / .cpp` — the pool (acquire, release, singleton)
- `main.cpp` — spawns client threads to test it
