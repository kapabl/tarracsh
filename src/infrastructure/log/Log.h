#ifndef KAPA_LOG_H
#define KAPA_LOG_H
#include <filesystem>
#include <mutex>
#include <string>



namespace kapa::infrastructure::log {

class Log {
public:
    Log() = default;

    void write(const std::string &value, bool doStdout = false);
    void writeln(const std::string &value, bool doStdout = false);
    void setFile(std::string logFile);
    void init(const std::string &logFile);
    static void emptyLogFile(const std::string &file);

private:
    std::string _logFile;
    std::mutex _mutex;
};

}

#endif
