#ifndef TARRACSH_JAR_DIGEST_H
#define TARRACSH_JAR_DIGEST_H
#include "../app/Tarracsh.h"
#include "../classfile/Stats.h"
#include <map>

#include "JarEntry.h"
#include "JarTask.h"
#include "../tables/FilesTable.h"
#include "../tables/ClassfilesTable.h"

namespace org::kapa::tarracsh::jar {

class JarDigestTask : public JarTask {
public:
    explicit JarDigestTask(
        Options options, stats::Results &results,
        std::shared_ptr<tables::ClassfilesTable> digestTable,
        std::shared_ptr<tables::FilesTable> filesTable
        );

    void processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) override;
    bool start() override;
    void end() override;

private:
    std::shared_ptr<tables::ClassfilesTable> _digestTable;
    std::shared_ptr<tables::FilesTable> _filesTable;
    stats::Results &_results;
    Options _options;
    bool _isFileUnchanged{false};
    bool _isNewJarFile{false};
    std::map<std::string, tables::DigestColumn> _digestMap;

    [[nodiscard]] std::optional<tables::DigestColumn> parseEntry(const JarEntry &jarEntry,
                                                              const tables::ClassfileRow *row) const;

    [[nodiscard]] const tables::ClassfileRow *getClassfileRow(const JarEntry &jarEntry) const;
    [[nodiscard]] static bool isClassfileUnchanged(const JarEntry &jarEntry, const tables::ClassfileRow *classRow);
    [[nodiscard]] bool isFileUnchanged() const;
    [[nodiscard]] const tables::FileRow *createJarFileRow(const std::string &filename) const;
    [[nodiscard]] const tables::FileRow *getJarFileRow(const std::string &filename);

};

}

#endif
