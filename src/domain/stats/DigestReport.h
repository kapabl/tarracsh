#ifndef TARRACSH_REPORT_H
#define TARRACSH_REPORT_H

#include <string>
#include <vector>

#include "../Options.h"
#include "../infrastructure/filesystem/Utils.h"


namespace kapa::tarracsh::domain::stats {
struct Results;
struct JarfileStats;
struct ClassfileStats;
}

namespace kapa::tarracsh::domain::stats::report {

struct FileResult {
    std::string filename;
    bool isNew{false};
    bool isModified{false};
    bool isSamePublicDigest{true};
    bool failed{false};
};

struct ClassResult {
    std::string strongClassname;
    bool isNew{false};
    bool isModified{false};
    bool isSamePublicDigest{false};
};

class DigestReport {
public:

    explicit DigestReport(Results& results);

    void asNewJar(const std::string &filename );
    void asNewClassfile(const std::string &filename, const std::string& strongClassname );
    void asModifiedJar(const std::string &filename, bool isSamePublicDigest);
    void asModifiedClassfile(const std::string &filename, bool isSamePublicDigest, const std::string& strongClassname);
    void asUnchangedJar(const std::string &filename);
    void asUnchangedClassfile(const std::string &filename);

    void asFailedJar(const std::string& filename);
    void asFailedClassfile(const std::string& filename);


    void asNewJarClass(const std::string &strongClassname);
    void asModifiedJarClass(const std::string &strongClassname, bool isSamePublicDigest);
    void asUnchangedJarClass(const std::string &strongClassname);


    void print() const;

    [[nodiscard]] const std::vector<FileResult>& getJarResults() const { return jarResults; }
    [[nodiscard]] const std::vector<ClassResult>& getClassResults() const { return classfileResults; }

private:
    Results& results;
    JarfileStats& jarfiles;
    ClassfileStats& classfiles;
    Options& options;
    std::vector<FileResult> jarResults;
    std::vector<ClassResult> classfileResults;

    std::mutex mutex;

    void asNewFile(const std::string& filename);
    void asModifiedFile(const std::string& filename, bool isSamePublicDigest);
    void asUnchangedFile(const std::string& filename);
    void asFailedFile(const std::string& filename);

    void asNewClass(const std::string& strongClassname);
    void asUnchangedClass(const std::string& strongClassname);
    void asModifiedClass(const std::string& strongClassname, bool isSamePublicDigest);
};


}

#endif
