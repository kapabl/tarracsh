#ifndef TARRACSH_JAR_TASK_H
#define TARRACSH_JAR_TASK_H

#include "../JarEntry.h"
#include "domain/Options.h"
#include "infrastructure/db/Database.h"
#include "domain/db/table/Classfiles.h"
#include "domain/db/table/Files.h"
#include "domain/stats/Results.h"

namespace kapa::tarracsh::domain::jar {
class Task {
public:
    virtual void processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) = 0;
    virtual db::table::FileRow* getFileRow(const std::string& filename);
    virtual std::shared_ptr<db::table::Files> getFileTable() = 0;
    virtual infrastructure::db::Database& getDb() = 0;

    virtual bool start() = 0;
    virtual void end() = 0;
    virtual uint32_t getCount();

    Task(Options options, stats::Results& results);
    virtual ~Task();

    [[nodiscard]] db::table::FileRow &getJarFileRow() const;

protected:

    db::table::FileRow* _tempFileRow{ nullptr };

    stats::Results& _results;
    Options _options;

    bool _isFileUnchanged{ false };
    bool _isNewJarFile{ false };

    db::table::FileRow *_jarFileRow = nullptr;
    uintmax_t _jarSize{};
    long long _jarTimestamp{};

    [[nodiscard]] static bool isClassfileUnchanged(const JarEntry &jarEntry, const db::table::ClassfileRow *classRow);

    [[nodiscard]] db::table::FileRow* getOrCreateFileRow(const std::string& filename);
    [[nodiscard]] db::table::FileRow *createJarFileRow(const std::string &filename);
    [[nodiscard]] bool isFileUnchanged() const;

};

}

#endif
