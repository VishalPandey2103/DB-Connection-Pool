#include "connection.h"
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

Connection::Connection() {
    id = -1;
    is_open = false;
}

void Connection::open(int id) {
    this->id = id;
    usleep(100000);
    is_open = true;
    cout << "[pool] connection " << id << " opened\n";
}

void Connection::execute(const char* query) {
    usleep(50000);
    cout << "    connection " << id << " ran query: " << query << "\n";
}

int Connection::get_id() {
    return id;
}