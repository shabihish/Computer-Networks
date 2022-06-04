#include "JsonReader.h"

namespace pt = boost::property_tree;
using namespace std;

void get_config_data_from_jsonFile(const string config_file_path,int* command_port,int* data_port, vector<UserInformation*>* users_info,
             vector<string>* files)
{
    namespace pt = boost::property_tree;

    pt::ptree config_file;                                                    
    pt::read_json(config_file_path, config_file);

    *command_port = config_file.get_child("commandChannelPort").get_value<int>();
    *data_port = config_file.get_child("dataChannelPort").get_value<int>();

    for (pt::ptree::value_type &user : config_file.get_child("users").get_child("")) {
         string name = user.second.get<string>("user");
         string password = user.second.get<string>("password");
         bool admin = user.second.get<bool>("admin");
         double size = user.second.get<double>("size");
         users_info->push_back(new UserInformation(name, password, admin, size));
    }

    for (pt::ptree::value_type &file : config_file.get_child("files").get_child(""))
        files->push_back(file.second.get_value<string>());
}


void get_ports_from_jsonFile(string config_file_path,int& command_port,int& data_port)
{
    
    pt::ptree config_file;                                                    
    pt::read_json(config_file_path, config_file);

    command_port = config_file.get_child("commandChannelPort").get_value<int>();
    data_port = config_file.get_child("dataChannelPort").get_value<int>();

}


