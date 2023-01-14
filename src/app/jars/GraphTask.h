#ifndef TARRACSH_JAR_GRAPH_H
#define TARRACSH_JAR_GRAPH_H
#include "../app/Tarracsh.h"
#include "../app/stats/Stats.h"
#include <map>

#include "JarEntry.h"
#include "Task.h"
#include "../db/columns/Columns.h"
#include "../db/CallGraphDb.h"
#include "../db/table/FilesTable.h"
#include "../db/table/ClassfilesTable.h"


namespace kapa::tarracsh::jar {

class GraphTask : public Task {
public:
    explicit GraphTask(
        Options options, stats::Results &results,
        db::callgraph::CallGraphDb& callGraphDb
        );

    void processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) override;
    bool start() override;
    void end() override;

private:
    db::callgraph::CallGraphDb& _callGraphDb;
    stats::Results &_results;
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
