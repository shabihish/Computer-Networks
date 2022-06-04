#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime> 

class Logger {
public:

    Logger(std::string path);
    void log(std::string msg);

private:
    std::string logfile_path;
};

#endif
