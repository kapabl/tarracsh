#ifndef KAPA_DB_H
#define KAPA_DB_H
#include <filesystem>
#include <string>
#include "StringPool.h"
#include "query/Engine.h"
#include "../log/Log.h"


namespace kapa::infrastructure::db {
namespace tables {
class Table;
}

class Database {

public:


    struct Config {
        std::string dataDir;
        log::Log* log{ nullptr };
    };

    explicit Database(const Config& config);
    virtual ~Database() = default;
    virtual void stop();
    tables::Table *getTable(const std::string &tablename);

    Database(const Database& other) = delete;
    Database(const Database&& other) = delete;
    Database* operator=(const Database& other) = delete;
    Database* operator=(const Database&& other) = delete;

    virtual void init();
    virtual void clean();
    virtual void backup();
    virtual bool read();
    virtual bool write();

    virtual void printSchema();
    [[nodiscard]] tables::columns::StringCol getPoolString(const std::string& value) const;
    std::shared_ptr<StringPool> getStringPool() { return _stringPool; }
    [[nodiscard]] std::string generateTableFilename(const std::string& name) const;
    [[nodiscard]] log::Log& log() const { return _log; }
    static bool init( Database& db, const bool doClean );
    bool executeQuery(const std::string &query, const bool displayRaw);


protected:

    [[nodiscard]] std::string generateStringPoolFilename(const std::string &name) const;
    std::shared_ptr<StringPool> _stringPool;
    std::unique_ptr<query::Engine> _queryEngine;
    log::Log& _log;
    std::unordered_map<std::string, tables::Table*> _tables;
    bool _read{false};

    Config _config;
};



}


#endif
