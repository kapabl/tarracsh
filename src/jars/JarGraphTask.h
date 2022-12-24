#ifndef TARRACSH_JAR_GRAPH_H
#define TARRACSH_JAR_GRAPH_H
#include "../app/Tarracsh.h"
#include "../app/stats/Stats.h"
#include <map>

#include "JarEntry.h"
#include "JarTask.h"
#include "../tables/CallGraphDb.h"
#include "../tables/FilesTable.h"
#include "../tables/ClassfilesTable.h"


namespace org::kapa::tarracsh::jar {

class JarGraphTask : public JarTask {
public:
    explicit JarGraphTask(
        Options options, stats::Results &results,
        db::CallGraphDb& callGraphDb
        );

    void processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) override;
    bool start() override;
    void end() override;

private:
    db::CallGraphDb& _callGraphDb;
    stats::Results &_results;
    Options _options;
    bool _isFileUnchanged{false};
    bool _isNewJarFile{false};
    std::map<std::string, db::tables::columns::DigestCol> _digestMap;

    [[nodiscard]] std::optional<db::tables::columns::DigestCol> parseEntry(const JarEntry &jarEntry,
                                                              const db::tables::ClassfileRow *row) const;

    [[nodiscard]] const db::tables::ClassfileRow *getClassfileRow(const JarEntry &jarEntry) const;
    [[nodiscard]] static bool isClassfileUnchanged(const JarEntry &jarEntry, const db::tables::ClassfileRow *classRow);
    [[nodiscard]] bool isFileUnchanged() const;
    [[nodiscard]] const db::tables::FileRow *createJarFileRow(const std::string &filename) const;
    [[nodiscard]] const db::tables::FileRow *getJarFileRow(const std::string &filename);

};

}

#endif
