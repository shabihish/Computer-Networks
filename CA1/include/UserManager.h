#ifndef USERMANAGER_H_
#define USERMANAGER_H_


#include "User.h"

#include <vector>
#include <algorithm>

#include <fstream>
#include "Config.h"
#define SLASH '/'

class UserManager
{
public:
    UserManager(std::vector<UserInformation*> users_info_);

    ~UserManager();

    void add_user(int command_socket, int data_socket);
    void remove_user(int socket);

    User* get_user_by_id(int socket);

    UserInformation* get_user_info(std::string username);
  
    bool user_exist_in_directory(std::string dir_path);


private:
    std::vector<UserInformation*> users_info;
    std::vector<User*> current_users;

};

#endif
