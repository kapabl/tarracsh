#ifndef TARRACSH_LOG_H
#define TARRACSH_LOG_H
#include <cassert>
#include <fstream>
#include <filesystem>
#include <mutex>
#include <string>
#include <utility>


namespace org::kapa::tarracsh::log {

class Log {
public:
    Log() = default;

    void write(const std::string &value);
    void writeln(const std::string &value);
    void setFile(std::string logFile);
    void init(const std::string logFile);
    static void emptyLogFile(const std::string &file);

private:
    std::string _logFile;
    std::mutex _mutex;
};

}

#endif
