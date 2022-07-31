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
        std::ofstream file(_logFile, std::ios_base::app);
        file << value;
    }

    void writeln(const std::string& value) {
        assert(!_logFile.empty());
        std::unique_lock<std::mutex> lock(_mutex);
        std::ofstream file(_logFile, std::ios_base::app);
        file << value << std::endl;
    }

    void setFile(std::string logFile) {
        _logFile = std::move(logFile);
    }
    
    static void emptyLogFile(const std::string& file) {
        std::ofstream logFile;
        logFile.open(file, std::ofstream::out | std::ofstream::trunc);
        logFile.close();
    }

private:
    std::string _logFile;
    std::mutex _mutex;
};

#endif
