#include "Log.h"

#include <cassert>
#include <iostream>
#include <fstream>


using namespace kapa::infrastructure::log;

void Log::write(const std::string &value, const bool doStdout) {
    assert(!_logFile.empty());
    std::unique_lock<std::mutex> lock(_mutex);
    std::ofstream file(_logFile, std::ios_base::app);
    file << value;
    if (doStdout) {
        std::cout << value;
    }

}

void Log::writeln(const std::string &value, const bool doStdout ) {
    auto valuePlusEndL = value;
    valuePlusEndL.push_back('\n');
    write(valuePlusEndL, doStdout);
    if ( doStdout ) {
        std::cout.flush();
    }

}

void Log::setFile(std::string logFile) {
    _logFile = std::move(logFile);
}

void Log::init(const std::string &logFile) {
    emptyLogFile(logFile);
    setFile(logFile);
}

void Log::emptyLogFile(const std::string &file) {
    if (std::filesystem::exists(file)) {
        resize_file(std::filesystem::path(file), 0);
    }
}
