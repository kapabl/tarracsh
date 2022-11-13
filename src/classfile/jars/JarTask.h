#ifndef TARRACSH_JAR_TASK_H
#define TARRACSH_JAR_TASK_H

#include "JarEntry.h"

namespace org::kapa::tarracsh::jar {
class JarTask {
public:

    virtual void processEntry(const JarEntry &jarEntry,
                              std::mutex &taskMutex) = 0;

    virtual bool start() = 0;
    virtual void end() = 0;

    virtual ~JarTask() = default;
};

}

#endif
