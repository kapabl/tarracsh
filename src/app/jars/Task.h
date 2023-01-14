#ifndef TARRACSH_JAR_TASK_H
#define TARRACSH_JAR_TASK_H

#include "JarEntry.h"
#include "../db/table/FilesTable.h"

namespace kapa::tarracsh::jar {
class Task {
public:
    virtual void processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) = 0;

    virtual bool start() = 0;
    virtual void end() = 0;
    virtual uint32_t getCount() { return _jarFileRow->classfileCount; }
    virtual ~Task() = default;

    [[nodiscard]] db::digest::FileRow &getJarFileRow() const { return *_jarFileRow; }

protected:
    db::digest::FileRow *_jarFileRow = nullptr;
    uintmax_t _jarSize{};
    long long _jarTimestamp{};
};

}

#endif
