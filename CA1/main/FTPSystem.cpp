#include "FTPSystem.h"

using namespace std;

FTPSystem::FTPSystem(UserManager* user_manager_, FileManager* file_manager_,  Logger* logger_) {
    user_manager = user_manager_;
    logger = logger_;
    file_manager = file_manager_;
}

FTPSystem::~FTPSystem() {
    delete user_manager;
    delete file_manager;
}

UserManager* FTPSystem::get_user_manager() {
    return user_manager;
}


void FTPSystem::add_new_user(int new_command_socket, int new_data_socket){
    user_manager->add_user(new_command_socket, new_data_socket);
}


vector<string> FTPSystem::tokenize(char* input) {
    
    vector<string> output_strings;

    char *word = strtok(input, " "); 
   
    while (word != NULL) {
        output_strings.push_back(word);
        word = strtok(NULL, " ");
    }
    return output_strings;
}



bool FTPSystem::user_has_access_to_this_file(string file_name, User* user) {
    if (!file_manager->file_has_administrator_permission(file_name))
        return true;
    else if (user->is_admin())
        return true;
    return false;
}

void FTPSystem::set_splitted_command(char* command){
    splitted_command = tokenize(command);
}

void FTPSystem::set_current_user(int user_socket){
    user = user_manager->get_user_by_id(user_socket);
}


bool FTPSystem::directory_can_be_deleted(std::string dir_path)
{
    string bash_command = "realpath " + dir_path  + " > file.txt";
    if (system(bash_command.c_str()) != 0)
        return false;
    string delet_path = file_manager->read_data_from_file("file.txt");
    if (system("rm file.txt") != 0)
        return false;

    if(!user_manager->user_exist_in_directory(delet_path))
        return true;
    return false;

}

vector<string> FTPSystem::handle_command( int user_socket, char* command) {

    if(!strcmp(command,""))
        return {GENERAL_ERROR, EMPTY};
       
    set_splitted_command(command);
    set_current_user(user_socket);  
    if (user == nullptr)
        return {GENERAL_ERROR, EMPTY};

    if (splitted_command[COMMAND] == USER_COMMAND) {
        if (user->get_UserStatus() == User::UserStatus::LOGGED_IN) 
            return {GENERAL_ERROR, EMPTY};
        return user_command();
    }

    else if (splitted_command[COMMAND] == PASS_COMMAND) {
        if (user->get_UserStatus() == User::UserStatus::LOGGED_IN) 
            return {GENERAL_ERROR, EMPTY};
        return pass_command();
    }

    else if (user->get_UserStatus() != User::UserStatus::LOGGED_IN) 
       return {NOT_AUTHORIZED, EMPTY};

    else if (splitted_command[COMMAND] == PWD_COMMAND) {
        return pwd_command();
    }

    else if (splitted_command[COMMAND] == MKD_COMMAND) {
        return mkd_command();
    }

    else if (splitted_command[COMMAND] == DELE_COMMAND && splitted_command[ARG1] == DELE_FILE) {
        return dele_f_command();   
    }

    else if (splitted_command[COMMAND] == DELE_COMMAND && splitted_command[ARG1] == DELE_DIRECTORY) {
        return dele_d_command();
    }

    else if (splitted_command[COMMAND] == LS_COMMAND) {
        return ls_command();
    }

    else if (splitted_command[COMMAND] == CWD_COMMAND) {
       
        return cwd_command();
    }

    else if (splitted_command[COMMAND] == RENAME_COMMAND) {
       
        return rename_command();
    }

    else if (splitted_command[COMMAND] == RETR_COMMAND) {
       
        return retr_command();
    }

    else if (splitted_command[COMMAND] == HELP_COMMAND) {
        return help_command();
    }

    else if (splitted_command[COMMAND] == QUIT_COMMAND) {
        return quit_command();
    }

    else
        return {GENERAL_ERROR, EMPTY};
}



vector<std::string> FTPSystem::user_command() {
    if (splitted_command.size() != 2)
        return {SYNTAX_ERROR, EMPTY};
    string username = splitted_command[ARG1];
    if(user->get_UserStatus() != User::UserStatus::WAITING_FOR_USERNAME)
        return {BAD_SEQUENCE, EMPTY};
    UserInformation* user_information = user_manager->get_user_info(username);
    if (user_information == nullptr)
        return {INVALID_USER_PASS, EMPTY};
    user->set_UserStatus(User::UserStatus::WAITING_FOR_PASSWORD);
    user->set_user_info(user_information);
    
    user->set_current_directory(ROOT);
    return {USERNAME_ACCEPTED, EMPTY};
}

vector<std::string> FTPSystem::pass_command() {

    if (splitted_command.size() != 2)
        return {SYNTAX_ERROR, EMPTY};

    string password = splitted_command[ARG1];
    if(user->get_UserStatus() != User::UserStatus::WAITING_FOR_PASSWORD)
        return {BAD_SEQUENCE, EMPTY};

    UserInformation *userinfo = user->get_user_info();
    if (userinfo->password != password)
        return {INVALID_USER_PASS, EMPTY};

    user->set_UserStatus(User::UserStatus::LOGGED_IN);

    logger->log(userinfo->username + COLON + LoggedinMsg);

    return {SUCCESSFUL_LOGIN, EMPTY};
}




vector<string> FTPSystem::pwd_command() {

    if (splitted_command.size() != 1)
            return {SYNTAX_ERROR, EMPTY};
    string current_path = user->get_current_directory();
    if (current_path == ROOT)
        current_path = ".";
        
    string bash_command = "realpath " + current_path + " > file.txt";
    if (system(bash_command.c_str()) != 0)
        return {GENERAL_ERROR, EMPTY};

    string data = file_manager->read_data_from_file("file.txt");
   
    if (system("rm file.txt") != 0)
        return {GENERAL_ERROR, EMPTY};
  
    return {PWD_CODE + data , EMPTY};
}


vector<string> FTPSystem::mkd_command() {

    if (splitted_command.size() != 2)
        return {SYNTAX_ERROR, EMPTY};

    string dir_path = splitted_command[ARG1];
    string bash_command = "mkdir " + user->get_current_directory() + dir_path;
    if (system(bash_command.c_str()) == 0) {
        string message = COLON + dir_path + CreateMsg;
        logger->log(user->get_user_info()->username + message);
        return {CREATE_CODE + message, EMPTY};
    }
    return {GENERAL_ERROR, EMPTY};
}


vector<string> FTPSystem::dele_f_command() {
    if (splitted_command.size() != 3)
        return {SYNTAX_ERROR, EMPTY};
    string file_name =  splitted_command[ARG2];

    if (file_name.find("/") != std::string::npos)//to check file name validity
        return {SYNTAX_ERROR, EMPTY};
    if (user_has_access_to_this_file(file_name, user) == false)
        return {FILE_UNAVAILABLE, EMPTY};
    string bash_command = "rm " + user->get_current_directory() + file_name;
    if (system(bash_command.c_str()) == 0) {
        string message = COLON + file_name + DeleteMsg;
        logger->log(user->get_username() + message);
        return {DELETE_CODE + message, EMPTY};
    }
    return {GENERAL_ERROR, EMPTY};
}


vector<string> FTPSystem::dele_d_command() {
    if (splitted_command.size() != 3)
        return {SYNTAX_ERROR, EMPTY};
    string dir_path =  splitted_command[ARG2];

    if(!file_manager->path_exists( dir_path))
        return {GENERAL_ERROR, EMPTY};
    
    if(!directory_can_be_deleted(dir_path))
        return {GENERAL_ERROR, EMPTY};

    string bash_command = "rm -r " + user->get_current_directory() + dir_path;
    if (system(bash_command.c_str()) == 0) {
        string message = COLON + dir_path + DeleteMsg;
        logger->log(user->get_username() + message);
        return {DELETE_CODE + message, EMPTY};
    }
    return {GENERAL_ERROR, EMPTY};
}

vector<string> FTPSystem::ls_command() {
    if (splitted_command.size() != 1)
        return {SYNTAX_ERROR, EMPTY};
    string bash_command = "ls " + user->get_current_directory() + " > file.txt";
   
    if (system(bash_command.c_str()) != 0)
        return {GENERAL_ERROR, EMPTY};
    string data = file_manager->read_data_from_file("file.txt");
    if(data == EMPTY){
        system("rm file.txt");
        return {LIST_TRANSFER_DONE, EMPTY};
    }
  
    if (system("rm file.txt") != 0)
        return {GENERAL_ERROR, EMPTY};
    size_t pos = data.find("file.txt\n");
    if (pos != std::string::npos) {
        data.erase(pos, strlen("file.txt\n"));
    }
    data = "\n" + data;
    return {LIST_TRANSFER_DONE, data};
}


std::vector<std::string> FTPSystem::cwd_command() {
    if (splitted_command.size() > 2)
        return {SYNTAX_ERROR, EMPTY};
    
    string dir_path;
    if(splitted_command.size() >= 2)
        dir_path = splitted_command[ARG1];
    else
        dir_path = "..";

    string  path = "realpath " + dir_path + " > file.txt";
   
    if (system(path.c_str()) != 0 ){
        system("rm file.txt");
        return {GENERAL_ERROR, EMPTY};}
     string absolute_pathname = file_manager->read_data_from_file("file.txt");

    bool dir_exists = file_manager->path_exists( absolute_pathname);
    bool remove_tmpfile = system("rm file.txt");
    if (dir_exists == 0 || remove_tmpfile != 0)
        return {GENERAL_ERROR, EMPTY};

    if(dir_path == "..")
        user->set_current_directory(ROOT);
    else
        user->set_current_directory(user->get_current_directory() + dir_path + "/");

    return {SUCCESSFUL_CHANGE, EMPTY};
}


std::vector<std::string> FTPSystem::rename_command() {

    if (splitted_command.size() != 3)
            return {SYNTAX_ERROR, EMPTY};

    string old_name = splitted_command[ARG1];
    string new_name =  splitted_command[ARG2];

    if ((old_name.find(BACK_SLASH) != std::string::npos) || (new_name.find(BACK_SLASH) != std::string::npos))
         return {SYNTAX_ERROR, EMPTY};

    if (!user_has_access_to_this_file(old_name, user))
        return {FILE_UNAVAILABLE, EMPTY};

    string bash_command = "mv " + user->get_current_directory() + old_name + " " +
            user->get_current_directory() + new_name;
    if (system(bash_command.c_str()) == 0)
        return {SUCCESSFUL_CHANGE, EMPTY};
    return {GENERAL_ERROR, EMPTY};
}

std::vector<std::string> FTPSystem::retr_command() {

    if (splitted_command.size() != 2)
            return {SYNTAX_ERROR, EMPTY};

    string file_name = splitted_command[ARG1];

    if (file_name.find(BACK_SLASH) != std::string::npos)
        return {SYNTAX_ERROR, EMPTY};
        
    if (!user_has_access_to_this_file(file_name, user))
        return {FILE_UNAVAILABLE, EMPTY};

    string file_path = user->get_current_directory() + file_name;
    string size_command = "stat -c%s " + file_path + " > " + "size.txt";
  
    if (system(size_command.c_str()) != 0)
        return {GENERAL_ERROR, EMPTY};
    
    double file_size = file_manager->get_file_size("size.txt");
    if (system("rm size.txt") != 0)
        return {GENERAL_ERROR, EMPTY};

    if (user -> can_download(file_size) == false)
        return {DOWNLOAD_LIMIT_SIZE, EMPTY};

    string bash_command = "cp " + file_path + " file.txt";
    if (system(bash_command.c_str()) != 0)
        return {GENERAL_ERROR, EMPTY};

    std::ifstream f("file.txt");
    std::string addr;

    f.seekg(0, std::ios::end);   
    addr.reserve(f.tellg());
    f.seekg(0, std::ios::beg);

    addr.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    if (system("rm file.txt") != 0)
        return {GENERAL_ERROR, EMPTY};

    user->decrease_available_size(file_size);

    string message = COLON + file_name + DownloadMsg;
    logger->log(user->get_user_info()->username + message);

    return {SUCCESSFUL_DOWNLOAD, addr};
}

std::vector<std::string> FTPSystem::help_command() {
    if (splitted_command.size() != 1)
        return {SYNTAX_ERROR, EMPTY};
    string info = HELP_CODE;
    info += "\n";

    for(auto &command_info : all_commands_descriptions)
        info += command_info; 
    return {info, EMPTY};
}

vector<string> FTPSystem::quit_command() {
    if (splitted_command.size() != 1)
            return {SYNTAX_ERROR, EMPTY};    
    if (user->get_UserStatus() != User::UserStatus::LOGGED_IN)
        return {GENERAL_ERROR, EMPTY};

    user->set_UserStatus(User::UserStatus::WAITING_FOR_USERNAME);

    logger->log(user->get_user_info()->username + COLON + LoggedoutMsg);

    return {SUCCESSFUL_QUIT, EMPTY};
}
