#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H

#include <pthread.h>
#include "connection.h"

#define MAX_CONNECTIONS 16

class ConnectionPool {
public:
    static ConnectionPool& getInstance();

    void init(int pool_size);
    Connection* acquire();
    void release(Connection* conn);
    void shutdown();

private:
    ConnectionPool();
    ConnectionPool(const ConnectionPool&);
    ConnectionPool& operator=(const ConnectionPool&);

    Connection connections[MAX_CONNECTIONS];
    int available[MAX_CONNECTIONS];
    int top;
    int pool_size;

    pthread_mutex_t lock;
    pthread_cond_t not_empty;
};

#endif