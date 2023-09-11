#ifndef TARRACSH_JAR_DIGEST_H
#define TARRACSH_JAR_DIGEST_H
#include <map>

#include "../JarEntry.h"
#include "DbBasedTask.h"
#include "domain/db/DigestDb.h"
#include "domain/db/table/ClassFiles.h"
#include "domain/classfile/ClassFileParser.h"
#include "domain/jar/JarEntryInfo.h"
#include "domain/digest/DigestUtils.h"
#include "domain/stats/Results.h"


namespace kapa::tarracsh::domain::jar::tasks {

class DigestTask : public DbBasedTask {
public:
    explicit DigestTask(
        Options options, stats::Results &results,
        db::digest::DigestDb &digestDb
        );

    ~DigestTask() override;


    void processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) override;
    bool start() override;
    void end() override;

    [[ nodiscard ]] db::digest::DigestDb &getDb() const { return _db; }

private:
    db::digest::DigestDb &_db;
    std::map<std::string, infrastructure::db::table::column::DigestCol> _digestMap;

    [[nodiscard]] std::optional<infrastructure::db::table::column::DigestCol> digestEntry(
        const JarEntryInfo &digestEntryInfo,
        const db::table::ClassFileRow *row);

    infrastructure::db::Database& getDb() override;

protected:
    auto getClassfiles() -> std::shared_ptr<db::table::ClassFiles> override;
    auto getFiles() -> std::shared_ptr<db::table::Files> override;

};

}

#endif
