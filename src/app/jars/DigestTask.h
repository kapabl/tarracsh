#ifndef TARRACSH_JAR_DIGEST_H
#define TARRACSH_JAR_DIGEST_H
#include <map>

#include "JarEntry.h"
#include "Task.h"
#include "../db/DigestDb.h"
#include "../db/columns/Columns.h"
#include "../db/table/ClassfilesTable.h"
#include "../domain/classfile/ClassFileAnalyzer.h"

namespace kapa::tarracsh::jar {
struct DigestEntryInfo;

class DigestTask : public Task {
public:
    explicit DigestTask(
        Options options, stats::Results &results,
        db::digest::DigestDb& digestDb
        );

    [[ nodiscard ]] std::string getStrongClassname(const JarEntry &jarEntry) const;

    void processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) override;
    void updateFileTableInMemory(kapa::tarracsh::digestUtils::DigestVector digest) const;
    bool start() override;
    void updateClassfileTableInMemory(const JarEntry &jarEntry, const db::digest::columns::DigestCol& result,
                  const ClassFileAnalyzer& classFileAnalyzer) const;
    void end() override;

    [[ nodiscard ]] db::digest::DigestDb& getDb() const { return _digestDb; }

private:
    db::digest::DigestDb& _digestDb;
    // std::shared_ptr<db::ClassfilesTable> _classfilesTable;
    // std::shared_ptr<db::FilesTable> _filesTable;
    stats::Results &_results;
    Options _options;
    bool _isFileUnchanged{false};
    bool _isNewJarFile{false};
    std::map<std::string, db::digest::columns::DigestCol> _digestMap;

    [[nodiscard]] std::optional<db::digest::columns::DigestCol> digestEntry(const DigestEntryInfo& digestEntryInfo,
                                                                            const db::digest::ClassfileRow *row) const;

    [[nodiscard]] static bool isClassfileUnchanged(const JarEntry &jarEntry, const db::digest::ClassfileRow *classRow);
    [[nodiscard]] bool isFileUnchanged() const;
    [[nodiscard]] const db::digest::FileRow *createJarFileRow(const std::string &filename) const;
    [[nodiscard]] const db::digest::FileRow *getOrCreateFileRow(const std::string &filename);

};

struct DigestEntryInfo {
    explicit DigestEntryInfo(const DigestTask& jarDigestTask, const JarEntry& jarEntry) :
        jarEntry(jarEntry),
        fileRow(jarDigestTask.getJarFileRow())
    {
        strongClassname = jarDigestTask.getDb().getClassfiles()->getStrongClassname(
            fileRow, jarEntry.getClassname().c_str());
    }
    const JarEntry& jarEntry;
    db::digest::FileRow& fileRow;
    std::string strongClassname;
};

}

#endif
