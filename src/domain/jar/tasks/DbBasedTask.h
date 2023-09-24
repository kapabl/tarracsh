#ifndef TARRACSH_DB_BASED_JAR_TASK_H
#define TARRACSH_DB_BASED_JAR_TASK_H

#include "domain/Options.h"
#include "infrastructure/db/Database.h"
#include "domain/db/table/Files.h"
#include "domain/stats/Results.h"
#include "Task.h"
#include "domain/classfile/ClassFileParser.h"

namespace kapa::tarracsh::domain::jar {
class DbBasedTask : public Task {
public:
    DbBasedTask(const Options &options, stats::Results &results);

    virtual infrastructure::db::Database &getDb() = 0;
    ~DbBasedTask() override;

    [[nodiscard]] db::table::FileRow &getJarFileRow() const;
    std::string getUniqueClassname(const JarEntry &jarEntry, const classfile::ClassFileParser &classFileParser);
    void updateFile(const digestUtils::DigestVector &digest);
    uint64_t addOrUpdateClassfile(const JarEntry &jarEntry, const classfile::ClassFileParser &classFileParser);
    uint64_t addOrUpdateClassfile(const JarEntry &jarEntry,
                                  const infrastructure::db::table::column::DigestCol &digestCol,
                                  const classfile::ClassFileParser &classFileParser);

protected:
    db::table::FileRow *_tempFileRow{nullptr};
    db::table::FileRow *_jarFileRow = nullptr;
    bool _isFileUnchanged{false};
    bool _isNewJarFile{false};
    uintmax_t _jarSize{};
    long long _jarTimestamp{};

    [[nodiscard]] db::table::FileRow *getOrCreateFileRow(const std::string &filename);
    [[nodiscard]] db::table::FileRow *createJarFileRow(const std::string &filename);
    virtual db::table::FileRow *getFileRow(const std::string &filename);

    [[nodiscard]] static bool isClassfileUnchanged(const JarEntry &jarEntry, const db::table::ClassFileRow *classRow);
    [[nodiscard]] bool isFileUnchanged() const;


    virtual auto  getClassFiles() -> std::shared_ptr<db::table::ClassFiles> = 0;
    virtual auto  getFiles() -> std::shared_ptr<db::table::Files> = 0;
};

}

#endif
