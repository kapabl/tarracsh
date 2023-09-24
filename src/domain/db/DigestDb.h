#ifndef TARRACSH_DIGEST_DB_H
#define TARRACSH_DIGEST_DB_H
#include <filesystem>

#include "table/ClassFiles.h"
#include "infrastructure/db/Database.h"
#include "infrastructure/db/table/Table.h"
#include "table/Files.h"


namespace kapa::tarracsh::domain::db::digest {


class DigestDb : public infrastructure::db::Database {
public:
    explicit DigestDb(const Config& config, const bool hasSaveThread)
        : Database(config, hasSaveThread) {
    }

    static std::shared_ptr<DigestDb> create(
        const Config& config,
        const bool doClean,
        const bool hasSaveThread);

    void init() override;
    auto getFiles() { return _files; }
    auto getClassFiles() { return _classfiles; }
private:
    std::shared_ptr<table::Files> _files;
    std::shared_ptr<table::ClassFiles> _classfiles;

};



}


#endif
