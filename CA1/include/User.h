#ifndef USER_H_
#define USER_H_


#include <iostream>
#include <string>

#include "Config.h"



struct UserInformation{
    std::string username,password;
    bool admin;
    double available_size;
    UserInformation( std::string name = "", std::string pass = "" ,bool admin_ = false, double size = 0 )
    {
        username = name;
        password = pass;
        admin = admin_;
        available_size = size;
    }

    UserInformation& operator=(const UserInformation& other){
        username = other.username;
        password = other.password;
        admin = other.admin;
        available_size = other.available_size;
        return *this;
    } 

};

class User {
public:
    enum UserStatus {
        WAITING_FOR_USERNAME,
        WAITING_FOR_PASSWORD,
        LOGGED_IN,
    };

    User(int command_socket, int data_socket);

    void set_UserStatus(UserStatus _UserStatus);
    void set_user_info(struct UserInformation* user_info);
    void set_current_directory(std::string path);

    int get_id();
    int get_data_socket();
    UserStatus get_UserStatus();
    std::string get_current_directory();
    struct UserInformation* get_user_info();
    std::string get_username();

  
    void decrease_available_size(double file_size);

    bool can_download(double file_size);
    bool is_admin();

private:
    int id;
    int data_socket;
    UserStatus Userstatus;
    std::string current_directory;
    UserInformation* UserInfo;

};

#endif
