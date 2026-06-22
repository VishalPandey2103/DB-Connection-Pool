#ifndef CONNECTION_H
#define CONNECTION_H

class Connection {
public:
    Connection();
    void open(int id);
    void execute(const char* query);
    int get_id();

private:
    int id;
    bool is_open;
};

#endif