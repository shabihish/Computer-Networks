#ifndef JsonReader_H_
#define JsonReader_H_



#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <boost/property_tree/ptree.hpp>                                        
#include <boost/property_tree/json_parser.hpp>

#include "User.h"




void get_config_data_from_jsonFile(const std::string config_file_path,int* command_port,int* data_port, std::vector<UserInformation*>* users_info,
             std::vector<std::string>* files);

void get_ports_from_jsonFile(std::string config_file_path,int& command_port,int& data_port);


#endif