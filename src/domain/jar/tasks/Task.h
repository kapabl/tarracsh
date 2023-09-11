#ifndef TARRACSH_JAR_TASK_H
#define TARRACSH_JAR_TASK_H

#include "../JarEntry.h"
#include "domain/Options.h"
#include "infrastructure/db/Database.h"
#include "domain/db/table/ClassFiles.h"
#include "domain/db/table/Files.h"
#include "domain/stats/Results.h"

namespace kapa::tarracsh::domain::jar {
class Task {
public:
    Task(Options options, stats::Results& results);
    virtual ~Task();

    virtual void processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) = 0;

    virtual bool start() = 0;
    virtual void end() = 0;


protected:

    stats::Results& _results;
    Options _options;



};

}

#endif
