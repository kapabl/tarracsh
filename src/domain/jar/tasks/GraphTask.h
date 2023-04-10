#ifndef TARRACSH_JAR_GRAPH_H
#define TARRACSH_JAR_GRAPH_H
#include "../../stats/Results.h"
#include <map>

#include "../JarEntry.h"
#include "Task.h"
#include "../../db/CallGraphDb.h"
#include "../../db/table/FilesTable.h"
#include "../../db/table/ClassfilesTable.h"


namespace kapa::tarracsh::domain::jar::tasks {

using kapa::tarracsh::domain::stats::Results;

class GraphTask : public Task {
public:
    explicit GraphTask(
        Options options, Results &results,
        db::callgraph::CallGraphDb &callGraphDb
        );

    void processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) override;
    bool start() override;
    void end() override;

private:
    db::callgraph::CallGraphDb &_callGraphDb;
    Results &_results;
    Options _options;
    bool _isFileUnchanged{false};
    bool _isNewJarFile{false};
    std::map<std::string, infrastructure::db::table::column::DigestCol> _digestMap;

    [[nodiscard]] std::optional<infrastructure::db::table::column::DigestCol> parseEntry(
        const JarEntry &jarEntry,
        const db::table::ClassfileRow *row) const;

    [[nodiscard]] const db::table::ClassfileRow *getClassfileRow(const JarEntry &jarEntry) const;
    [[nodiscard]] bool isFileUnchanged() const;
    [[nodiscard]] const db::table::FileRow *createJarFileRow(const std::string &filename) const;
    [[nodiscard]] const db::table::FileRow *getJarFileRow(const std::string &filename);

};

}

#endif
