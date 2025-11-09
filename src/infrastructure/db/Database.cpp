#include "Database.h"

#include <string>
#include <filesystem>
#include <chrono>
#include <memory>
#include <ranges>
#include <memory>
#include <condition_variable>
#include "StringPool.h"

using namespace kapa::infrastructure::db;
using namespace table;
using namespace std::chrono_literals;


kapa::infrastructure::profiler::MillisecondDuration Database::getReadTime() const {
    return _readTime;
}


Database::Database(const Config &config, const bool hasSaveThread)
    : _log(*config.log), _config(config), _hasSaveThread(hasSaveThread) {
    if (_hasSaveThread) {
        createSaveThread();
    }
}

void Database::createSaveThread() {

    _saveThread = std::jthread([this](const std::stop_token &stopToken) -> void {
        std::mutex mutex;
        std::unique_lock lock(mutex);
        while (true) {
            std::condition_variable_any().wait_for(lock, stopToken, 10s, [&stopToken] {
                return stopToken.stop_requested();
            });

            if (stopToken.stop_requested()) {
                break;
            }
            write();
        }

    });
}


void Database::stop() {
    if (_saveThread.get_id() != std::jthread::id()) {
        _saveThread.request_stop();
    }
    write();
}

void Database::init() {
    _stringPool = std::make_shared<db::StringPool>(generateStringPoolFilename("sp"));
}

column::StringCol Database::getPoolString(const std::string &value) const {
    const auto result = _stringPool->add(value);
    return result;
}

namespace {
constexpr std::string_view kTableExtension = ".kapamd";
}

std::string Database::generateTableFilename(const std::string &name) const {
    std::string result(name + std::string(kTableExtension));
    result = (std::filesystem::path(_config.dataDir) / result).string();

    return result;
}

bool Database::init(Database &db, const bool doClean) {
    auto result = true;
    db.init();

    if (doClean) {
        db.backup();
        db.clean();
    } else {
        result = db.read();
    }

    return result;
}

std::string Database::generateStringPoolFilename(const std::string &name) const {
    std::string result(name + StringPoolExtension);
    result = (std::filesystem::path(_config.dataDir) / result).string();

    return result;
}
