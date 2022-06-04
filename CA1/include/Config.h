#ifndef Config_H_
#define Config_H_



#define COMMAND 0
#define CHANNEL 1
#define ROOT ""
#define CONNECTED true
#define DISCONNECTED false
#define ARG1 1
#define ARG2 2
#define EMPTY " "
#define COLON ": "
#define ROOT ""
#define BACK_SLASH "/"

//size
#define BacklogQueueSize 5
#define BUFFSize 1024


//file paths
#define LOG_FILE "log.txt"
#define ConfigFilePath "config.json"

//command codes
#define DELETE_CODE "250 "
#define CREATE_CODE "257 "
#define HELP_CODE "214: "
#define PWD_CODE "257: "

//command
//namespace command {
     const std::string USER_COMMAND = "user";
     const std::string PASS_COMMAND = "pass";
     const std::string PWD_COMMAND = "pwd";
     const std::string MKD_COMMAND = "mkd";
     const std::string DELE_COMMAND = "dele";
     const std::string DELE_DIRECTORY = "-d";
     const std::string DELE_FILE = "-f";
     const std::string LS_COMMAND = "ls";
     const std::string CWD_COMMAND = "cwd";
     const std::string RENAME_COMMAND = "rename";
     const std::string RETR_COMMAND = "retr";
     const std::string HELP_COMMAND = "help";
     const std::string QUIT_COMMAND = "quit";



// #define USER_COMMAND "user"
// #define PASS_COMMAND "pass"
// #define PWD_COMMAND "pwd"
// #define MKD_COMMAND "mkd"
// #define DELE_COMMAND "dele"
// #define LS_COMMAND "ls"
// #define CWD_COMMAND "cwd"
// #define RENAME_COMMAND "rename"
// #define RETR_COMMAND "retr"
// #define HELP_COMMAND "help"
// #define QUIT_COMMAND "quit"

// #define DELE_DIRECTORY "-d"
// #define DELE_FILE "-f"

//command outputs
#define USERNAME_ACCEPTED "331: User name okay, need password."
#define BAD_SEQUENCE "503: Bad sequence of commands."
#define SUCCESSFUL_LOGIN "230: User logged in, proceed. Logged out if appropriate."
#define INVALID_USER_PASS "430: Invalid username or password"
#define FILE_UNAVAILABLE "550: File unavailable."
#define LIST_TRANSFER_DONE "226: List transfer done."
#define SUCCESSFUL_CHANGE "250: Successful change."
#define SUCCESSFUL_DOWNLOAD "226: Successful Download."
#define SUCCESSFUL_QUIT "221: Successful Quit."
#define NOT_AUTHORIZED "332: Need account for login."
#define SYNTAX_ERROR "501: Syntax error in parameters or arguments."
#define GENERAL_ERROR "500: Error"
#define DOWNLOAD_LIMIT_SIZE "425: Can't open data connection."

//info for logger file
#define LoggedinMsg " logged in."
#define LoggedoutMsg " logged out."
#define DownloadMsg " downloaded."
#define DeleteMsg " deleted."
#define CreateMsg " created."

//commands descriptions
const std::string USER_DESCRIPTION = "USER [name]\n\t Its argument is used to specify the user's string. It is used for user authentication.\n\n";
const std::string PASS_DESCRIPTION = "PASS [password]\n\t Its argument is used to specify the user's password. It is used for user authentication.\n\n";
const std::string PWD_DESCRIPTION = "PWD\n\t It is used for printing current directory.\n\n";
const std::string MKD_DESCRIPTION = "MKD [path]\n\t Its argument is used to specify the directory's path. It is usede to create a new directory.\n\n";
const std::string DELE_F_DESCRIPTION = "DELE -f [name]\n\t Its argument is used to specify the file's name. It is usede to remove a file.\n\n";
const std::string DELE_D_DESCRIPTION = "DELE -d [name]\n\t Its argument is used to specify the directory's name. It is usede to remove a directory.\n\n";
const std::string LS_DESCRIPTION = "LS\n\t It is used to print the list of files in the current directory.\n\n";
const std::string CWD_DESCRIPTION = "CWD [path]\n\t Its argument is used to specify the new directory's path. It is used to change the current directory.\n\n";
const std::string RENAME_DESCRIPTION = "RENAME [from] [to]\n\t Its arguments are used to specify the old file's name(from) and new file's name(to). It is used to change a file's name.\n\n";
const std::string RETR_DESCRIPTION = "RETR [name]\n\t Its argument is used to specify the file's name. It is used to download a file.\n\n";
const std::string HELP_DESCRIPTION = "HELP\n\t It is used to pritn commands information.\n\n";
const std::string QUIT_DESCRIPTION = "QUIT\n\t It is used to signing out from server.\n\n";
const std::string all_commands_descriptions[] = {USER_DESCRIPTION, PASS_DESCRIPTION,PWD_DESCRIPTION,MKD_DESCRIPTION,DELE_F_DESCRIPTION,
         DELE_D_DESCRIPTION,LS_DESCRIPTION,CWD_DESCRIPTION, RENAME_DESCRIPTION, RETR_DESCRIPTION,
         HELP_DESCRIPTION,QUIT_DESCRIPTION};


#endif