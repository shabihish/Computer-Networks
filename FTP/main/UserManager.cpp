#include "UserManager.h"

using namespace std;

UserManager::UserManager(std::vector<UserInformation*> users_info_)
{
    users_info = users_info_;
}


UserManager::~UserManager() {
    for (auto info : users_info)
        delete info;
    users_info.clear();

    for (auto user : current_users)
        delete user;
    current_users.clear();
}

void UserManager::add_user(int command_socket, int data_socket) {
    current_users.push_back(new User(command_socket,  data_socket));
}

#include <fstream>
bool UserManager::user_exist_in_directory(std::string dir_path)
{
    for(auto &user :  current_users)
    {
        string current_path = user->get_current_directory();
        if (current_path == ROOT)
            current_path = ".";
        string bash_command = "realpath " + current_path + " > file.txt";
        if (system(bash_command.c_str()) != 0)
            return -1;
        std::ifstream f("file.txt");
        if(f.peek() == EOF)
            return " ";
        std::string data;
        f.seekg(0, std::ios::end);   
        data.reserve(f.tellg());
        f.seekg(0, std::ios::beg);
        data.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        data.pop_back();
        if (system("rm file.txt") != 0)
            return -1;
        if( dir_path == data)
            return 1;
    }

    return 0;
  
}


void UserManager::remove_user(int socket) {
    for(size_t i = 0; i < current_users.size(); ++i) {
        if (current_users[i]->get_id() == socket) {
            current_users.erase(current_users.begin() + i);
            break;
        }
    }
}


User* UserManager::get_user_by_id(int socket) {
    for(size_t i = 0; i < current_users.size(); ++i)
        if (current_users[i]->get_id() == socket)
            return current_users[i];
    return nullptr;
}


UserInformation* UserManager::get_user_info(string username) {
    for(size_t i = 0; i < users_info.size(); ++i)
        if (users_info[i]->username == username)
            return users_info[i];
    return nullptr;
}

