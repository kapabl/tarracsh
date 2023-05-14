#include "Log.h"

#include <cassert>
#include <iostream>
#include <fstream>


using namespace kapa::infrastructure::log;

void Log::write(const std::string &value, const bool doStdout, const std::string &prefix, const std::string &postfix) {
    assert(!_logFile.empty());
    std::unique_lock<std::mutex> lock(_mutex);
    std::ofstream file(_logFile, std::ios_base::app);
    file << value;
    if (doStdout || _forceStdout) {
        std::cout << prefix << value << postfix;
    }
}

void Log::writeln(const std::string &value, const bool doStdout) {
    auto valuePlusEndL = value;
    valuePlusEndL.push_back('\n');
    write(valuePlusEndL, doStdout);
    if (doStdout || _forceStdout) {
        std::cout.flush();
    }
}

void Log::writelnRed(const std::string &value) {
    auto valuePlusEndL = value;
    valuePlusEndL.push_back('\n');
    write(valuePlusEndL, true, "\033[31m", "\033[0m");
    std::cout.flush();

}

void Log::writelnGreen(const std::string &value) {
    auto valuePlusEndL = value;
    valuePlusEndL.push_back('\n');
    write(valuePlusEndL, true, "\033[32m", "\033[0m");
    std::cout.flush();
}

void Log::setFile(const std::string &logFile) {
    _logFile = logFile;
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
