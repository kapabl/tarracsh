#ifndef TARRACSH_JAR_TASK_H
#define TARRACSH_JAR_TASK_H
#include "../Tarracsh.h"
#include "../Stats.h"
#include <map>

#include "JarEntry.h"
#include "../tables/DigestTable.h"

namespace org::kapa::tarracsh::jar {
class JarTask {
public:
    virtual void processEntry(const JarEntry &jarEntry,
                              std::mutex &taskMutex) = 0;

    virtual bool start() = 0;
    virtual void end() = 0;
};

class JarDigestTask : public JarTask {
public:
    explicit JarDigestTask(
        Options options, Results &results,
        std::shared_ptr<tables::DigestTable> digestTable);

    void processEntry(const JarEntry &jarEntry, std::mutex &taskMutex) override;
    bool start() override;
    void end() override;

private:
    std::shared_ptr<tables::DigestTable> _digestTable;
    Results &_results;
    Options _options;
    bool _isFileUnchanged{false};
    bool _isExistingJar{false};
    tables::MD5 _digest;
    tables::DigestRow *_jarRow = nullptr;
    uintmax_t _jarSize{};
    long long _jarTimestamp{};
    std::map<std::string, tables::MD5> _digestMap;

    [[nodiscard]] std::optional<tables::MD5> digestEntry(const JarEntry &jarEntry) const;
    std::optional<tables::MD5> parseEntry(const JarEntry &jarEntry, const tables::DigestRow* row) const;
    tables::DigestRow *getClassfileRow(const JarEntry &jarEntry) const;
    bool isClassfileUnchanged(const JarEntry &jarEntry, const tables::DigestRow *classRow) const;
    bool isFileUnchanged() const;

};

}

#endif
