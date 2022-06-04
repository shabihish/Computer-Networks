#ifndef FTPSystem_H_
#define FTPSystem_H_

#include "UserManager.h"
#include "FileManager.h"

#include "User.h"

#include "Logger.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <sys/stat.h>

#include "Config.h"

class FTPSystem {
public:

    FTPSystem(UserManager* user_manager_, FileManager* file_manager,  Logger* logger_);
    ~FTPSystem();
    UserManager* get_user_manager();
    void add_new_user(int new_command_socket, int new_data_socket);
    std::vector<std::string> handle_command(int user_socket, char* command);

private:

    void set_splitted_command(char* command);
    void set_current_user(int user_socket);
    bool user_has_access_to_this_file(std::string file_name, User* user);
    std::vector<std::string> tokenize(char* input);
    bool directory_can_be_deleted(std::string dir_path);
    

    std::vector<std::string> user_command();
    std::vector<std::string> pass_command();
    std::vector<std::string> pwd_command();
    std::vector<std::string> mkd_command();
    std::vector<std::string> dele_f_command();
    std::vector<std::string> dele_d_command();
    std::vector<std::string> ls_command();
    std::vector<std::string> cwd_command();
    std::vector<std::string> rename_command();
    std::vector<std::string> retr_command();
    std::vector<std::string> help_command();
    std::vector<std::string> quit_command();
    

    UserManager* user_manager;
    FileManager* file_manager;
    Logger* logger;
    User* user;
    std::vector<std::string> splitted_command;

};

#endif
