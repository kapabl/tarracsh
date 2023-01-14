#ifndef TARRACSH_DIGEST_DB_H
#define TARRACSH_DIGEST_DB_H
#include <filesystem>
#include <string>

#include "table/ClassfilesTable.h"
#include "../infrastructure/db/Database.h"
#include "table/FilesTable.h"


namespace kapa::tarracsh::db::digest {


class DigestDb : public infrastructure::db::Database {
public:
    DigestDb(const std::string &dataDir, infrastructure::log::Log& log)
        : Database(dataDir, log) {
    }

    void init() override;
    void clean() override;
    bool read() override;
    bool write() override;

    void printSchema() override;

    auto getFiles() { return _filesTable; }
    std::shared_ptr<ClassfilesTable> getClassfiles() { return _classfilesTable; }
    void outputStats() const;
private:
    std::shared_ptr<FilesTable> _filesTable;
    std::shared_ptr<ClassfilesTable> _classfilesTable;
};



}


#endif
