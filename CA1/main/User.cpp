#include "User.h"

using namespace std;

User::User(int command_socket, int data_socket_)
{
    id = command_socket;
    data_socket = data_socket_;
    Userstatus = WAITING_FOR_USERNAME;
    current_directory = ROOT;
    UserInfo = new struct UserInformation;

}


void User::set_UserStatus(User::UserStatus _UserStatus) {
    Userstatus = _UserStatus;
}

void User::set_user_info(struct UserInformation* user_info) {
    UserInfo = user_info;
}

void User::set_current_directory(string path) {
    current_directory = path;
}


int User::get_id() {
    return id;
}

int User::get_data_socket() {
    return data_socket;
}

User::UserStatus User::get_UserStatus() {
    return Userstatus;
}


string User::get_current_directory() {
    return current_directory;
}

UserInformation* User::get_user_info() {
    return UserInfo;
}

string User::get_username() {
    return UserInfo->username;
}

bool User::can_download(double file_size) {
    if (UserInfo->available_size >= file_size/1000)
        return true;
    return false;
}

void User::decrease_available_size(double file_size) {
   UserInfo->available_size -= (file_size/1000);
}

bool User::is_admin() {
    return UserInfo->admin;
}