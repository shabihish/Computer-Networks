#include "Client.h"

#define localhostIP "127.0.0.1"
#define RecvCommandSize 4096
#define RecvDataSize 4096
static constexpr int MAX_COMMAND_LENGTH = 128;
using namespace std;


Client::Client(const string config_file_path){

    get_ports_from_jsonFile(config_file_path,command_channel_port, data_channel_port);


}

void Client::create_sockets()
{
    command_fd = socket(AF_INET, SOCK_STREAM, 0);
    data_fd = socket(AF_INET, SOCK_STREAM, 0);
    if( command_fd < 0 )
    {
        perror("command socket failed");
        exit(EXIT_FAILURE);
    }
    if( data_fd < 0)
    {
        perror("data socket failed");
        exit(EXIT_FAILURE);
    }
   
}

void Client::connect_to_server()
{
    struct sockaddr_in server_command_address;
    server_command_address.sin_family = AF_INET;
    server_command_address.sin_port = htons(command_channel_port);
    if (inet_pton(AF_INET, localhostIP, &server_command_address.sin_addr) <= 0)
    {
        perror("error occurred in converting network address\n");
        exit(EXIT_FAILURE);
    }   

    struct sockaddr_in server_data_address;
    server_data_address.sin_family = AF_INET;
    server_data_address.sin_port = htons(data_channel_port);
    if (inet_pton(AF_INET, localhostIP, &server_data_address.sin_addr) <= 0)
    {
        perror("error occurred in converting network address\n");
        exit(EXIT_FAILURE);
    }

    if (connect(command_fd, (struct sockaddr*)&server_command_address, sizeof(server_command_address)) < 0)
    {
        perror("connection to server failed");
        exit(EXIT_FAILURE);
    }
       

    if (connect(data_fd, (struct sockaddr*)&server_data_address, sizeof(server_data_address)) < 0)
    {
        perror("connection to server failed");
        exit(EXIT_FAILURE);
    }
       
}


void Client::start() {
   

    char received_command_output[RecvCommandSize] = {0};
    char received_data_output[RecvDataSize] = {0};
    while (true) {
        // Receive command from command line.
        cout << "> ";
        char command[MAX_COMMAND_LENGTH];
        memset(command, 0, MAX_COMMAND_LENGTH);
        cin.getline (command, MAX_COMMAND_LENGTH);

        // Send command to server.
        send(command_fd, command, MAX_COMMAND_LENGTH, 0);

        // Receive command output.
        memset(received_command_output, 0, sizeof received_command_output);
        recv(command_fd, received_command_output, sizeof(received_command_output), 0);
        cout << "Command channel output: " << received_command_output << endl;

        // Receive data output.
        memset(received_data_output, 0, sizeof received_data_output);
        recv(data_fd, received_data_output, sizeof(received_data_output), 0);
        cout << "Data channel output: " << received_data_output << endl;
    }
}

void Client::run()
{
    create_sockets();
    connect_to_server();
    start();
}
int main() {
    
    Client client(ConfigFilePath);
    client.run();
    return 0;
}
