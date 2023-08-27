#ifndef TARRACSH_JAR_GRAPH_H
#define TARRACSH_JAR_GRAPH_H

#include <map>
#include "domain/stats/Results.h"
#include "domain/jar/JarEntry.h"
#include "DbBasedTask.h"
#include "domain/db/CallGraphDb.h"
#include "domain/db/table/Files.h"
#include "domain/db/table/Classfiles.h"
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

    auto getClassfiles() -> std::shared_ptr<db::table::Classfiles> override;
    auto getFiles() -> std::shared_ptr<db::table::Files> override;



    void updateIncompleteRefs(const db::table::ClassfileRow* row);
    void processNewClassfile(const JarEntryInfo& jarEntryInfo);
    auto getClassfileRow(uint64_t id) -> const db::table::ClassfileRow*;
    void processIncompleteClassfile(const JarEntryInfo& jarEntryInfo, db::table::ClassfileRow* row);
    void markMemberRefsAsIncomplete(db::table::ClassfileRow * row);
    void deleteMembers(db::table::ClassfileRow * row);
    void reProcessClassfile(const JarEntryInfo& jarEntryInfo, db::table::ClassfileRow* row);

    void processClassfile(const JarEntryInfo& jarEntryInfo, db::table::ClassfileRow* row);
    static auto isIncompleteClassfileRow(const db::table::ClassfileRow *classfileRow) -> bool;
    auto findIncompleteClass(const JarEntryInfo& jarEntryInfo) -> db::table::ClassfileRow*;
    
};


}

#endif
