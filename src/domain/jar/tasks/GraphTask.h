#ifndef TARRACSH_JAR_GRAPH_H
#define TARRACSH_JAR_GRAPH_H
#include "../../stats/Results.h"
#include <map>

#include "../JarEntry.h"
#include "DbBasedTask.h"
#include "domain/db/CallGraphDb.h"
#include "domain/db/table/Files.h"
#include "domain/db/table/Classfiles.h"
#include "domain/classfile/ClassFileParser.h"
#include "domain/jar/JarEntryInfo.h"


namespace kapa::tarracsh::domain::jar::tasks {

using kapa::tarracsh::domain::stats::Results;

class GraphTask : public DbBasedTask {
public:
    explicit GraphTask(
        Options options, Results &results,
        db::callgraph::CallGraphDb &callGraphDb
        );

    void processClassfile(const JarEntryInfo &jarEntryInfo, const db::table::ClassfileRow *row);
    void processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) override;
    bool start() override;
    void end() override;

private:
    db::callgraph::CallGraphDb &_db;
;
    [[nodiscard]] const db::table::ClassfileRow *getClassfileRow(const JarEntry &jarEntry) const;

    infrastructure::db::Database& getDb() override;
    std::shared_ptr<db::table::Files> getFileTable() override;

    void recordMethod(const classfile::constantpool::MethodInfo& value);
    void recordClassMethods(classfile::ClassFileParser& classFileParser);
    void recordMethodRef(const classfile::constantpool::ConstantPoolRecord& entry);
    void recordClassRef(const classfile::constantpool::ConstantPoolRecord& entry);
    void recordInterfaceMethodRef(const classfile::constantpool::ConstantPoolRecord& entry);
    void recordFieldRef(const classfile::constantpool::ConstantPoolRecord & entry);
    void recordRefs(classfile::ClassFileParser& classFileParser);





};

}

#endif
