#include "connection_pool.h"
#include <iostream>
#include <string>
#include <pthread.h>

using namespace std;

ConnectionPool pool;

void* client_thread(void* arg) {
    long id = (long)arg;
    cout << "client " << id << ": requesting a connection...\n";
    Connection* conn = pool.acquire();
    cout << "client " << id << ": GOT connection " << conn->get_id() << "\n";
    conn->execute("SELECT * FROM users WHERE id = 1");
    pool.release(conn);
    cout << "client " << id << ": released connection " << conn->get_id() << "\n";
    return 0;
}

int main(int argc, char* argv[]) {
    int pool_size = 4;
    int num_clients = 10;
    if (argc >= 2) pool_size = stoi(argv[1]);
    if (argc >= 3) num_clients = stoi(argv[2]);
    if (pool_size < 1) pool_size = 1;
    if (pool_size > MAX_CONNECTIONS) pool_size = MAX_CONNECTIONS;
    if (num_clients > 256) num_clients = 256;

    cout << "initializing pool with " << pool_size
         << " connections for " << num_clients << " clients\n";
    pool.init(pool_size);

    pthread_t clients[256];
    long i;
    for (i = 0; i < num_clients; i++)
        pthread_create(&clients[i], 0, client_thread, (void*)i);
    for (i = 0; i < num_clients; i++)
        pthread_join(clients[i], 0);

    pool.shutdown();
    cout << "all clients done; pool shut down cleanly\n";
    return 0;
}