
#include "FileManager.h"
#include <dirent.h>
using namespace std;

FileManager::FileManager(vector<string> files_names)
{
    administrators_files = files_names;
}


 FileManager::~FileManager() {

    administrators_files.clear();
 }



bool FileManager::file_has_administrator_permission(string filename) {
    for (size_t i = 0; i < administrators_files.size(); i++) {
        if (administrators_files[i] == filename)
            return true;
    }
    return false;
}

double FileManager::get_file_size(std::string file_path) {
    std::ifstream ifile(file_path, std::ios::in);
    double size;
    ifile >> size;
    return size;
}

 std::string FileManager::read_data_from_file(std::string file_name)
 {
    std::ifstream f(file_name);
    if(f.peek() == EOF)
        return " ";
    std::string data;
    f.seekg(0, std::ios::end);   
    data.reserve(f.tellg());
    f.seekg(0, std::ios::beg);
    data.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    data.pop_back();
    return data;
  
 }



 bool FileManager::path_exists( std::string path)
 {
     
    DIR* dir = opendir(path.c_str());
    if (dir) 
        return true;
    
    else if (ENOENT == errno) 
        return false;
    return false;
    
 }