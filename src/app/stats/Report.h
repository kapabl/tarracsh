#ifndef TARRACSH_REPORT_H
#define TARRACSH_REPORT_H

#include <string>
#include <vector>

#include "../Options.h"
#include "../infrastructure/filesystem/FilesystemUtils.h"


namespace kapa::tarracsh::stats {
struct Results;
}

namespace kapa::tarracsh::stats::report {

struct JarResult {
    std::string filename;
    bool isNew{false};
    bool isModified{false};
    bool isSamePublicDigest{true};
};

struct ClassfileResult {
    std::string filename;
    std::string classname;
    bool isNew{false};
    bool isModified{false};
    bool isSamePublicDigest{false};
};

struct Report {
    Results &results;
    Options &options;
    std::vector<JarResult> jarResults;
    std::vector<ClassfileResult> classfileResults;

    std::mutex mutex;

    explicit Report(Results &results);

    void asNew(const std::string &filename);
    void asModified(const std::string &filename, bool isSamePublicDigest);
    void asUnchanged(const std::string &filename);

    void asNewClass(const std::string &fullClassname);
    void asModifiedClass(const std::string &fullClassname, bool isSamePublicDigest);
    void asUnchangedClass(const std::string &fullClassname);

    void print() const;
};


}

#endif
