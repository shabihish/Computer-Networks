#ifndef CLIENT_H_
#define CLIENT_H_

//#include "Config.h"

#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <vector>
#include <string>
#include "JsonReader.h"

class Client {
public:
    Client(const std::string config_file_path);
    void run();
   

private:
    void create_sockets();
    void connect_to_server();
    void start();

    int command_fd;
    int data_fd;
    int command_channel_port;
    int data_channel_port;
    
};

#endif
