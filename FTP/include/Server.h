#ifndef SERVER_H_
#define SERVER_H_

#include <vector>
#include <string>
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


#include "FTPSystem.h"
#include "Logger.h"
#include "User.h"
#include "UserManager.h"
#include "FileManager.h"
#include "Config.h"
#include "JsonReader.h"

class Server {
public:
    Server(const std::string config_file_path);
    ~Server();
    void run();
private:
    void start();
    void create_sockets();
    void bind_sockets();
    void sockets_listen();
    int accept_new_client();
    void communicate_with_client(int fd,  bool &connection_status);
    int command_port;
    int data_port;
    struct sockaddr_in command_addr;
    struct sockaddr_in data_addr;
    int data_fd;
    int command_fd;
    FTPSystem* ftp_system;
};

#endif
