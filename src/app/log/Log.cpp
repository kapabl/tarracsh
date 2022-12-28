#include "Log.h"


using namespace org::kapa::tarracsh::log;

void Log::write(const std::string &value) {
    assert(!_logFile.empty());
    std::unique_lock<std::mutex> lock(_mutex);
    std::ofstream file(_logFile, std::ios_base::app);
    file << value;
}

void Log::writeln(const std::string &value) {
    assert(!_logFile.empty());
    std::unique_lock<std::mutex> lock(_mutex);
    std::ofstream file(_logFile, std::ios_base::app);
    file << value << std::endl;
}

void Log::setFile(std::string logFile) {
    _logFile = std::move(logFile);
}

void Log::init(const std::string logFile) {
    emptyLogFile(logFile);
    setFile(logFile);
}

void Log::emptyLogFile(const std::string &file) {
    if (std::filesystem::exists(file)) {
        resize_file(std::filesystem::path(file), 0);
    }
}
