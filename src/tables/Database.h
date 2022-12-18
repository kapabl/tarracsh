#ifndef TARRACSH_DB_H
#define TARRACSH_DB_H
#include <filesystem>
#include <string>
#include "StringPool.h"


namespace org::kapa::tarracsh::db {

class Database {

public:

    explicit Database(const std::string& dataDir);
    virtual ~Database() = default;

    virtual void init();
    virtual void clean();
    virtual bool read();
    virtual bool write();

    virtual void printSchema() = 0;


    [[nodiscard]] db::tables::columns::StringCol getPoolString(const std::string& value) const;

    std::shared_ptr<db::StringPool> getStringPool() { return _stringPool; }
    [[nodiscard]] std::string generateTableFilename(const std::string& name) const;

protected:

    [[nodiscard]] std::string generateStringPoolFilename(const std::string &name) const;
    std::shared_ptr<db::StringPool> _stringPool;
    std::string _dataDir;
};



}


#endif
