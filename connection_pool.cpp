#include "connection_pool.h"

ConnectionPool& ConnectionPool::getInstance() {
    static ConnectionPool instance;
    return instance;
}

ConnectionPool::ConnectionPool() {
    top = 0;
    pool_size = 0;
    pthread_mutex_init(&lock, 0);
    pthread_cond_init(&not_empty, 0);

    for(int i = 0; i < MAX_CONNECTIONS; i++) {
        available[i] = -1;
    }
}

void ConnectionPool::init(int pool_size) {
    this->pool_size = pool_size;
    int i;

    for (i = 0; i < pool_size; i++) {
        connections[i].open(i);
        available[i] = i;
    }

    top = pool_size;
}

Connection* ConnectionPool::acquire() {

    pthread_mutex_lock(&lock);

    while (top == 0) {
        pthread_cond_wait(&not_empty, &lock);
    }
    top--;
    int idx = available[top];

    pthread_mutex_unlock(&lock);

    return &connections[idx];
}

void ConnectionPool::release(Connection* conn) {

    pthread_mutex_lock(&lock);
    
    available[top] = conn->get_id();
    top++;
    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&lock);
}

void ConnectionPool::shutdown() {
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&not_empty);
}