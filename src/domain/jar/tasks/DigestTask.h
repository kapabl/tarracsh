#ifndef TARRACSH_JAR_DIGEST_H
#define TARRACSH_JAR_DIGEST_H
#include <map>

#include "../JarEntry.h"
#include "Task.h"
#include "../domain/db/DigestDb.h"
#include "../domain/db/columns/Columns.h"
#include "../domain/db/table/ClassfilesTable.h"
#include "../domain/classfile/ClassFileParser.h"
#include "../domain/digest/DigestEntryInfo.h"
#include "../domain/digest/DigestUtils.h"

namespace kapa::tarracsh::domain::jar::tasks {

class DigestTask : public Task {
public:
    explicit DigestTask(
        Options options, stats::Results &results,
        db::digest::DigestDb &digestDb
        );

    void processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) override;
    bool start() override;
    void end() override;

    [[ nodiscard ]] db::digest::DigestDb &getDb() const { return _digestDb; }

private:
    db::digest::DigestDb &_digestDb;
    stats::Results &_results;
    Options _options;
    bool _isFileUnchanged{false};
    bool _isNewJarFile{false};
    std::map<std::string, db::digest::columns::DigestCol> _digestMap;
    std::unique_ptr<db::digest::FileRow> _pNewJarFileRow{};

    [[nodiscard]] std::optional<db::digest::columns::DigestCol> digestEntry(
        const digest::DigestJarEntryInfo &digestEntryInfo,
        const db::digest::ClassfileRow *row) const;

    [[nodiscard]] static bool isClassfileUnchanged(const JarEntry &jarEntry, const db::digest::ClassfileRow *classRow);
    [[nodiscard]] bool isFileUnchanged() const;
    [[nodiscard]] const db::digest::FileRow *createJarFileRow(const std::string &filename);
    [[nodiscard]] const db::digest::FileRow *getOrCreateFileRow(const std::string &filename);

    void updateFileTableInMemory(const digestUtils::DigestVector& digest) const;

    static std::string getUniqueClassname(
        const JarEntry& jarEntry,
        const classfile::ClassFileParser& classFileParser);
    void updateClassfileTableInMemory(const JarEntry& jarEntry, const db::digest::columns::DigestCol& result,
        const classfile::ClassFileParser& classFileParser) const;

};

}

#endif
