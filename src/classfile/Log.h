#ifndef TARRACSH_LOG_H
#define TARRACSH_LOG_H
#include <cassert>
#include <fstream>
#include <mutex>
#include <string>
#include <utility>

class Log {
public:
    Log() = default;

    void write(const std::string &value ) {
        assert(!_logFile.empty());
        std::unique_lock<std::mutex> lock(_mutex);
        std::ofstream file(_logFile);
        file << value;
    }

    void writeln(const std::string& value) {
        assert(!_logFile.empty());
        std::unique_lock<std::mutex> lock(_mutex);
        std::ofstream file(_logFile);
        file << value << std::endl;
    }

    void setFile(std::string logFile) {
        _logFile = std::move(logFile);
    }

private:
    std::string _logFile;
    std::mutex _mutex;
};

#endif
