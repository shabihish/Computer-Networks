#include "Server.h"




using namespace std;


Server::Server(const string config_file_path){
    std::vector<UserInformation*> users_info;
    std::vector<std::string> files;
    get_config_data_from_jsonFile(config_file_path, &command_port, &data_port, &users_info, &files);
    ftp_system = new FTPSystem(new UserManager(users_info), new FileManager(files), new Logger(LOG_FILE) );

}


Server::~Server() {
    delete ftp_system;
}


void Server::create_sockets()
{
    int opt = 1;
    if((command_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("command socket failed");
        exit(EXIT_FAILURE);
    }
    if((data_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("data socket failed");
        exit(EXIT_FAILURE);
    }
   
    if (setsockopt(data_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("set data socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(command_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("set command socket failed");
        exit(EXIT_FAILURE);
    }
   
}

void Server::bind_sockets()
{
   
    data_addr.sin_family = AF_INET; 
    data_addr.sin_addr.s_addr = INADDR_ANY; 
    data_addr.sin_port = htons(data_port);
    if (bind(data_fd, (struct sockaddr *)&data_addr, sizeof(data_addr)) < 0) 
    {
        perror("binding data socket failed");
        exit(EXIT_FAILURE);
    }

    command_addr.sin_family = AF_INET;
    command_addr.sin_addr.s_addr = INADDR_ANY;
    command_addr.sin_port = htons(command_port);
    if (bind(command_fd, (struct sockaddr *)&command_addr, sizeof(command_addr)) < 0) 
    {
        perror("binding command socket failed");
        exit(EXIT_FAILURE);
    }
        
   
}

void Server::sockets_listen(){
 if (listen(command_fd, BacklogQueueSize) < 0)
    {
        perror("data listening failed");
        exit(EXIT_FAILURE);
    }
        
    if (listen(data_fd, BacklogQueueSize) < 0)
    {
        perror("command listening failed");
        exit(EXIT_FAILURE);
    }
}



int Server::accept_new_client()
{
    int new_commandSock,new_dataSock;
   
                   
    new_commandSock = accept(command_fd,NULL,NULL);
    if (new_commandSock < 0)
    {
        perror("command channel accept failed");
        exit(EXIT_FAILURE);
    }

    new_dataSock = accept(data_fd, NULL, NULL);
    if (new_dataSock < 0)
    {
        perror("data channel accept failed");
        exit(EXIT_FAILURE);
    }
        
                
    ftp_system->add_new_user(new_commandSock,new_dataSock);
    cout<<"New client connected. data socket = "<< new_dataSock<<" command socket = "<< new_commandSock<<endl;
    return new_commandSock;
}


void Server::communicate_with_client(int fd, bool &connection_status)
{
    char recv_buffer[BUFFSize] = {0};
    memset(recv_buffer, 0, sizeof(recv_buffer));
    int recvMsgSize = recv(fd, recv_buffer, sizeof(recv_buffer), 0);
    if (recvMsgSize < 0){
        if (errno != EWOULDBLOCK)
            connection_status = DISCONNECTED;   
    }
    if (recvMsgSize == 0){
       connection_status = DISCONNECTED;
    }
   
    if (recvMsgSize > 0) {
   
        cout<<"command received from client "<<ftp_system->get_user_manager()->get_user_by_id(fd)->get_username()<<endl;       
        vector<string> connection_result = ftp_system->handle_command(fd, recv_buffer);
        send(fd , connection_result[COMMAND].c_str() , connection_result[COMMAND].size() , 0);
        send(ftp_system->get_user_manager()->get_user_by_id(fd)->get_data_socket(),
        connection_result[CHANNEL].c_str() , connection_result[CHANNEL].size() , 0);
        cout<<"answer sent to client "<<ftp_system->get_user_manager()->get_user_by_id(fd)->get_username()<<endl<<endl<<endl;
    }
}

void Server::start() {
   

    fd_set working_set, master_set;
    FD_ZERO(&master_set);
    FD_SET(command_fd, &master_set);
    int max_fd = command_fd;

   
    cout<<"Server is running\n";

	cout<<"Server started listenting on port "<< command_port <<" ...........\n";
    while (true) {
        
        working_set = master_set;
     
        int activity = select(max_fd + 1, &working_set, NULL, NULL, NULL);

        if (activity < 0)
            return;
       
        for (int fd = 0; fd <= max_fd ; ++fd) {
            if (FD_ISSET(fd, &working_set)) {
                // New connection.
                if (fd == command_fd) {
                    int new_commandSock = accept_new_client();
                    FD_SET(new_commandSock, &master_set);
                    if (new_commandSock > max_fd)
                        max_fd = new_commandSock;
  
                }
                // client sending and receiving msg.
                else {
                    bool connection_status = CONNECTED;
                    communicate_with_client(fd, connection_status);
                    //ending connection
                    if (connection_status == DISCONNECTED) {
                        cout<<"User "<< ftp_system->get_user_manager()->get_user_by_id(fd)->get_username()<<" disconnected\n";
                        close(fd);
                        close(ftp_system->get_user_manager()->get_user_by_id(fd)->get_data_socket());
                        ftp_system->get_user_manager()->remove_user(fd);
                        FD_CLR(fd, &master_set);
                        if (fd == max_fd)
                            while (FD_ISSET(max_fd, &master_set) == 0)
                                max_fd -= 1;
                    }

                }
            }
        }
    }
}

void Server::run()
{
    
    create_sockets();
    bind_sockets();
    sockets_listen();
    start();
}

int main() {

    Server server(ConfigFilePath);
    server.run();
    return 0;
}
