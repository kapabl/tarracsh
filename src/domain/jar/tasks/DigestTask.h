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

    [[ nodiscard ]] std::string getStrongClassname(const JarEntry &jarEntry) const;

    void processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) override;
    void updateFileTableInMemory(digestUtils::DigestVector digest) const;
    bool start() override;
    void updateClassfileTableInMemory(const JarEntry &jarEntry, const db::digest::columns::DigestCol &result,
                                      const classfile::ClassFileParser &classFileParser) const;
    void end() override;

    [[ nodiscard ]] db::digest::DigestDb &getDb() const { return _digestDb; }

private:
    db::digest::DigestDb &_digestDb;
    // std::shared_ptr<db::ClassfilesTable> _classfilesTable;
    // std::shared_ptr<db::FilesTable> _filesTable;
    stats::Results &_results;
    Options _options;
    bool _isFileUnchanged{false};
    bool _isNewJarFile{false};
    std::map<std::string, db::digest::columns::DigestCol> _digestMap;

    [[nodiscard]] std::optional<db::digest::columns::DigestCol> digestEntry(
        const digest::DigestJarEntryInfo &digestEntryInfo,
        const db::digest::ClassfileRow *row) const;

    [[nodiscard]] static bool isClassfileUnchanged(const JarEntry &jarEntry, const db::digest::ClassfileRow *classRow);
    [[nodiscard]] bool isFileUnchanged() const;
    [[nodiscard]] const db::digest::FileRow *createJarFileRow(const std::string &filename) const;
    [[nodiscard]] const db::digest::FileRow *getOrCreateFileRow(const std::string &filename);

};

}

#endif
