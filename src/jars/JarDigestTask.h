#ifndef TARRACSH_JAR_DIGEST_H
#define TARRACSH_JAR_DIGEST_H
#include "../app/Tarracsh.h"
#include "../app/Stats.h"
#include <map>

#include "JarEntry.h"
#include "JarTask.h"
#include "../tables/Table.h"
#include "../tables/DigestDb.h"
#include "../tables/ClassfilesTable.h"
#include "../classfile/ClassFileAnalyzer.h"

namespace org::kapa::tarracsh::jar {
struct DigestEntryInfo;

class JarDigestTask : public JarTask {
public:
    explicit JarDigestTask(
        Options options, stats::Results &results,
        db::DigestDb& digestDb
        );

    [[ nodiscard ]] std::string getStrongClassname(const JarEntry &jarEntry) const;

    void processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) override;
    void updateFileTableInMemory(org::kapa::tarracsh::digestUtils::DigestVector digest) const;
    bool start() override;
    void updateClassfileTableInMemory(const JarEntry &jarEntry, const db::tables::columns::DigestCol& result,
                  const ClassFileAnalyzer& classFileAnalyzer) const;
    void end() override;

    db::DigestDb& getDb() const { return _digestDb; }

private:
    db::DigestDb& _digestDb;
    // std::shared_ptr<db::ClassfilesTable> _digestTable;
    // std::shared_ptr<db::FilesTable> _filesTable;
    stats::Results &_results;
    Options _options;
    bool _isFileUnchanged{false};
    bool _isNewJarFile{false};
    std::map<std::string, db::tables::columns::DigestCol> _digestMap;

    [[nodiscard]] std::optional<db::tables::columns::DigestCol> digestEntry(const DigestEntryInfo& digestEntryInfo,
                                                                            const db::tables::ClassfileRow *row) const;

    [[nodiscard]] static bool isClassfileUnchanged(const JarEntry &jarEntry, const db::tables::ClassfileRow *classRow);
    [[nodiscard]] bool isFileUnchanged() const;
    [[nodiscard]] const db::tables::FileRow *createJarFileRow(const std::string &filename) const;
    [[nodiscard]] const db::tables::FileRow *getOrCreateFileRow(const std::string &filename);

};

struct DigestEntryInfo {
    explicit DigestEntryInfo(const JarDigestTask& jarDigestTask, const JarEntry& jarEntry) :
        jarEntry(jarEntry),
        fileRow(jarDigestTask.getJarFileRow())
    {
        strongClassname = jarDigestTask.getDb().getClassfiles()->getStrongClassname(
            fileRow, jarEntry.getClassname().c_str());
    }
    const JarEntry& jarEntry;
    db::tables::FileRow& fileRow;
    std::string strongClassname;
};

}

#endif
