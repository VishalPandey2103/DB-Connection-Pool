#include "connection.h"
#include <cstdio>
#include <unistd.h>

Connection::Connection() {
    id = -1;
    is_open = false;
}

void Connection::open(int id) {
    this->id = id;
    usleep(100000);
    is_open = true;
    printf("[pool] connection %d opened\n", id);
}

void Connection::execute(const char* query) {
    usleep(50000);
    printf("    connection %d ran query: %s\n", id, query);
}

int Connection::get_id() {
    return id;
}