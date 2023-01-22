#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <netdb.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Header/MyRedis.h"

#define DEF_PORT 1601
#define BUFFERSIZE 1024
#define CLIENT_EXIT '#'

class Server {
public:
    Server() = delete;
    ~Server() = delete;

    static void run();
    
private:
    static bool is_client_connected(const char*);

};

#endif // SERVER_H