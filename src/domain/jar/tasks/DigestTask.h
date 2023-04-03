#ifndef TARRACSH_JAR_DIGEST_H
#define TARRACSH_JAR_DIGEST_H
#include <map>

#include "../JarEntry.h"
#include "Task.h"
#include "../../db/DigestDb.h"
#include "../../db/columns/Columns.h"
#include "../../db/table/ClassfilesTable.h"
#include "../../classfile/ClassFileParser.h"
#include "../../digest/DigestEntryInfo.h"
#include "../../digest/DigestUtils.h"

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
    stats::Results &_results;
    Options _options;
    bool _isFileUnchanged{false};
    bool _isNewJarFile{false};
    std::map<std::string, infrastructure::db::tables::columns::DigestCol> _digestMap;
    db::digest::FileRow * _tempFileRow{nullptr};

    [[nodiscard]] std::optional<infrastructure::db::tables::columns::DigestCol> digestEntry(
        const digest::DigestJarEntryInfo &digestEntryInfo,
        const db::digest::ClassfileRow *row) const;

    [[nodiscard]] static bool isClassfileUnchanged(const JarEntry &jarEntry, const db::digest::ClassfileRow *classRow);
    [[nodiscard]] bool isFileUnchanged() const;
    [[nodiscard]] db::digest::FileRow *createJarFileRow(const std::string &filename) const;
    [[nodiscard]] db::digest::FileRow *getOrCreateFileRow(const std::string &filename);

    void updateFileTableInMemory(const digestUtils::DigestVector& digest) const;

    static std::string getUniqueClassname(
        const JarEntry& jarEntry,
        const classfile::ClassFileParser& classFileParser);
    void updateClassfileTableInMemory(const JarEntry& jarEntry, const infrastructure::db::tables::columns::DigestCol& result,
        const classfile::ClassFileParser& classFileParser) const;

};

}

#endif
