#ifndef TARRACSH_JAR_GRAPH_H
#define TARRACSH_JAR_GRAPH_H

#include <map>
#include "domain/stats/Results.h"
#include "domain/jar/JarEntry.h"
#include "DbBasedTask.h"
#include "domain/db/CallGraphDb.h"
#include "domain/db/table/Files.h"
#include "domain/db/table/ClassFiles.h"
#include "domain/classfile/ClassFileParser.h"
#include "domain/jar/JarEntryInfo.h"

namespace kapa::tarracsh::domain::jar::tasks {

using stats::Results;

class GraphTask final : public DbBasedTask {
public:
    explicit GraphTask(
        const Options& options,
        Results &results,
        db::callgraph::CallGraphDb &callGraphDb
        );


    void processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) override;
    bool start() override;
    void end() override;

private:
    db::callgraph::CallGraphDb &_db;

    infrastructure::db::Database& getDb() override;

    auto getClassFiles() -> std::shared_ptr<db::table::ClassFiles> override;
    auto getFiles() -> std::shared_ptr<db::table::Files> override;
   // void processNewClassfile(const JarEntryInfo& jarEntryInfo);
    auto getClassFileRow(uint64_t id) -> const db::table::ClassFileRow*;
    void processClassFile(const JarEntryInfo& jarEntryInfo, db::table::ClassFileRow* row);

};


}

#endif
