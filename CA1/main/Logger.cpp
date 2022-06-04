
#include "Logger.h"

using namespace std;

Logger::Logger(string logfile_path)
: logfile_path(logfile_path) {
    fstream log_file;
    log_file.open(logfile_path, fstream::in | fstream::out | fstream::app);
    if (!log_file) {
        cout << "Creating new log file..";
        log_file.open(logfile_path,  fstream::in | fstream::out | fstream::trunc);
        log_file.close();
    } 
}

void Logger::log(string message) {
     fstream log_file;
     log_file.open(logfile_path, fstream::in | fstream::out | fstream::app);
     auto curr = chrono::system_clock::now();
     time_t curr_time = chrono::system_clock::to_time_t(curr);
     log_file << ctime(&curr_time);
     log_file << message << endl;
     log_file.close();
}
