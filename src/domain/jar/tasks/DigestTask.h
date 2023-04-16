#ifndef TARRACSH_JAR_DIGEST_H
#define TARRACSH_JAR_DIGEST_H
#include <map>

#include "../JarEntry.h"
#include "Task.h"
#include "domain/db/DigestDb.h"
#include "domain/db/table/Classfiles.h"
#include "domain/classfile/ClassFileParser.h"
#include "domain/jar/JarEntryInfo.h"
#include "domain/digest/DigestUtils.h"

namespace kapa::tarracsh::domain::jar::tasks {

class DigestTask : public Task {
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
        const db::table::ClassfileRow *row) const;
    void updateFileTableInMemory(const digestUtils::DigestVector& digest) const;

    static std::string getUniqueClassname(
        const JarEntry& jarEntry,
        const classfile::ClassFileParser& classFileParser);
    void updateClassfileTableInMemory(const JarEntry& jarEntry, const infrastructure::db::table::column::DigestCol& result,
        const classfile::ClassFileParser& classFileParser) const;

    std::shared_ptr<db::table::Files> getFileTable() override;
    infrastructure::db::Database& getDb() override;

};

}

#endif
