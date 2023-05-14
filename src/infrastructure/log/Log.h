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
    void setFile(const std::string& logFile);
    void init(const std::string &logFile);
    static void emptyLogFile(const std::string &file);


    void forceStdout(const bool forceStdout ) { _forceStdout = forceStdout; }

private:
    std::string _logFile{"app.log"};
    std::mutex _mutex;
    bool _forceStdout{false};
};

}

#endif
