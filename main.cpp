#include "connection_pool.h"
#include <cstdio>
#include <cstdlib>
#include <pthread.h>

ConnectionPool pool;

void* client_thread(void* arg) {
    long id = (long)arg;
    printf("client %ld: requesting a connection...\n", id);
    Connection* conn = pool.acquire();
    printf("client %ld: GOT connection %d\n", id, conn->get_id());
    conn->execute("SELECT * FROM users WHERE id = 1");
    pool.release(conn);
    printf("client %ld: released connection %d\n", id, conn->get_id());
    return 0;
}

int main(int argc, char* argv[]) {
    int pool_size = 4;
    int num_clients = 10;
    if (argc >= 2) pool_size = atoi(argv[1]);
    if (argc >= 3) num_clients = atoi(argv[2]);
    if (pool_size < 1) pool_size = 1;
    if (pool_size > MAX_CONNECTIONS) pool_size = MAX_CONNECTIONS;
    if (num_clients > 256) num_clients = 256;

    printf("initializing pool with %d connections for %d clients\n",
           pool_size, num_clients);
    pool.init(pool_size);

    pthread_t clients[256];
    long i;
    for (i = 0; i < num_clients; i++)
        pthread_create(&clients[i], 0, client_thread, (void*)i);
    for (i = 0; i < num_clients; i++)
        pthread_join(clients[i], 0);

    pool.shutdown();
    printf("all clients done; pool shut down cleanly\n");
    return 0;
}