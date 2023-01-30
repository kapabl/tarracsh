#ifndef KAPA_DB_H
#define KAPA_DB_H
#include <filesystem>
#include <string>
#include "StringPool.h"
#include "../log/Log.h"


namespace kapa::infrastructure::db {

class Database {

public:

    explicit Database(const std::string& dataDir, log::Log& log);
    virtual ~Database() = default;
    virtual void stop();

    Database(const Database& other) = delete;
    Database(const Database&& other) = delete;
    Database* operator=(const Database& other) = delete;
    Database* operator=(const Database&& other) = delete;

    virtual void init();
    virtual void clean();
    virtual void backup();
    virtual bool read();
    virtual bool write();

    virtual void printSchema() = 0;


    [[nodiscard]] tables::columns::StringCol getPoolString(const std::string& value) const;

    std::shared_ptr<StringPool> getStringPool() { return _stringPool; }
    [[nodiscard]] std::string generateTableFilename(const std::string& name) const;

    [[nodiscard]] log::Log& log() const { return _log; }

    static bool init( Database& db, const bool doClean );

protected:

    [[nodiscard]] std::string generateStringPoolFilename(const std::string &name) const;
    std::shared_ptr<StringPool> _stringPool;
    std::string _dataDir;
    log::Log& _log;
};



}


#endif
