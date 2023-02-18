#ifndef TARRACSH_DIGEST_DB_H
#define TARRACSH_DIGEST_DB_H
#include <filesystem>
#include <string>

#include "table/ClassfilesTable.h"
#include "../infrastructure/db/Database.h"
#include "../infrastructure/db/table/Table.h"
#include "table/FilesTable.h"


namespace kapa::tarracsh::domain::db::digest {


class DigestDb : public infrastructure::db::Database {
public:
    explicit DigestDb(const Config& config, const bool hasSaveThread)
        : Database(config), _hasSaveThread( hasSaveThread ) {
    }

    static std::shared_ptr<DigestDb> create(
        const Config& config,
        const bool doClean,
        const bool hasSaveThread);

    void init() override;
    void stop() override;
    void backup() override;
    auto getFiles() { return _filesTable; }
    std::shared_ptr<ClassfilesTable> getClassfiles() { return _classfilesTable; }
    void outputStats() const;
private:
    std::shared_ptr<FilesTable> _filesTable;
    std::shared_ptr<ClassfilesTable> _classfilesTable;
    std::atomic_bool _stopSaveThread;
    bool _hasSaveThread;
    std::jthread _saveThread;

    void createSaveThread();

};



}


#endif
