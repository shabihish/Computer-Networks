#ifndef FileManager_H_
#define FileManager_H_

#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <string>
#include <fstream>
#include <streambuf>

#include <cstring>

class FileManager
{
public:
    FileManager(std::vector<std::string> files_names );

    ~FileManager();

    bool file_has_administrator_permission(std::string file_path);
    double get_file_size(std::string file_path);
     std::string read_data_from_file(std::string file_name);
    bool path_exists(std::string path_name);

private:
   
    std::vector<std::string> administrators_files;
};



#endif