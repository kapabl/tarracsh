#ifndef TARRACSH_JAR_GRAPH_H
#define TARRACSH_JAR_GRAPH_H
#include "../../stats/Results.h"
#include <map>

#include "../JarEntry.h"
#include "Task.h"
#include "../domain/db/columns/Columns.h"
#include "../domain/db/CallGraphDb.h"
#include "../domain/db/table/FilesTable.h"
#include "../domain/db/table/ClassfilesTable.h"


namespace kapa::tarracsh::domain::jar::tasks {

using kapa::tarracsh::domain::stats::Results;

class GraphTask : public Task {
public:
    explicit GraphTask(
        Options options, Results &results,
        db::callgraph::CallGraphDb& callGraphDb
        );

    void processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) override;
    bool start() override;
    void end() override;

private:
    db::callgraph::CallGraphDb& _callGraphDb;
    Results &_results;
    Options _options;
    bool _isFileUnchanged{false};
    bool _isNewJarFile{false};
    std::map<std::string, db::digest::columns::DigestCol> _digestMap;

    [[nodiscard]] std::optional<db::digest::columns::DigestCol> parseEntry(const JarEntry &jarEntry,
                                                                           const db::digest::ClassfileRow *row) const;

    [[nodiscard]] const db::digest::ClassfileRow *getClassfileRow(const JarEntry &jarEntry) const;
    [[nodiscard]] static bool isClassfileUnchanged(const JarEntry &jarEntry, const db::digest::ClassfileRow *classRow);
    [[nodiscard]] bool isFileUnchanged() const;
    [[nodiscard]] const db::digest::FileRow *createJarFileRow(const std::string &filename) const;
    [[nodiscard]] const db::digest::FileRow *getJarFileRow(const std::string &filename);

};

}

#endif
