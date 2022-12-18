#ifndef TARRACSH_DIGEST_DB_H
#define TARRACSH_DIGEST_DB_H
#include <filesystem>
#include <string>

#include "ClassfilesTable.h"
#include "Database.h"
#include "FilesTable.h"


namespace org::kapa::tarracsh::db {


class DigestDb : public Database {
public:
    DigestDb(const std::string &dataDir)
        : Database(dataDir) {

    }

    void init() override;
    void clean() override;
    bool read() override;
    bool write() override;

    void printSchema() override;

    auto getFiles() { return _filesTable; }
    std::shared_ptr<tables::ClassfilesTable> getClassfiles() { return _digestTable; }
private:
    std::shared_ptr<tables::FilesTable> _filesTable;
    std::shared_ptr<tables::ClassfilesTable> _digestTable;
};



}


#endif
