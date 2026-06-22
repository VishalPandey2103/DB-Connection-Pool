#include "connection_pool.h"

ConnectionPool::ConnectionPool() {
    pool_size = 0;
    pthread_mutex_init(&lock, 0);
    pthread_cond_init(&not_empty, 0);
}

void ConnectionPool::init(int pool_size) {
    this->pool_size = pool_size;

    for (int i = 0; i < pool_size; i++) {
        connections[i].open(i);
        available.push(&connections[i]);
    }
}

Connection* ConnectionPool::acquire() {
    pthread_mutex_lock(&lock);

    while (available.empty()) {
        pthread_cond_wait(&not_empty, &lock);
    }

    Connection* conn = available.top();
    available.pop();

    pthread_mutex_unlock(&lock);
    return conn;
}

void ConnectionPool::release(Connection* conn) {
    pthread_mutex_lock(&lock);

    available.push(conn);
    pthread_cond_signal(&not_empty);

    pthread_mutex_unlock(&lock);
}

void ConnectionPool::shutdown() {
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&not_empty);
}
