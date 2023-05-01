#ifndef TARRACSH_REPORT_H
#define TARRACSH_REPORT_H

#include <string>
#include <vector>

// #include "Results.h"
#include "../Options.h"
#include "infrastructure/filesystem/Utils.h"


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
    bool failed{false};
};

class Report {
public:
    explicit Report(Results &results);
    BaseOptions &getBaseOptions() const;

    void asNewJar(const std::string &filename);
    void asModifiedJar(const std::string &filename, bool isSamePublicDigest);
    void asUnchangedJar(const std::string &filename);
    void asFailedJar(const std::string& filename);

    void asNewClassfile(const std::string &strongClassname);
    void asModifiedClassfile(bool isSamePublicDigest, const std::string &strongClassname);
    void asUnchangedClassfile(const std::string &filename);
    void asFailedClassfile(const std::string &filename);

    void asNewJarClass(const std::string &strongClassname);
    void asModifiedJarClass(const std::string &strongClassname, bool isSamePublicDigest);
    void asUnchangedJarClass(const std::string &strongClassname);
    void asFailedJarClass(const std::string &strongClassname);


    void print() const;

    [[nodiscard]] const std::vector<FileResult> &getFileResults() const { return _fileResults; }
    [[nodiscard]] const std::vector<ClassResult> &getClassResults() const { return _classResults; }
    
    void addClass(const ClassResult& classResult) { _classResults.push_back(classResult); }
    void addFile(const FileResult& addResult) { _fileResults.push_back(addResult); }

private:
    Results &_results;
    JarfileStats &_jarfiles;
    ClassfileStats &_classfiles;
    Options &_options;
    std::vector<FileResult> _fileResults;
    std::vector<ClassResult> _classResults;

    std::mutex _mutex;

    void addNewFile(const std::string& filename);
    void addModifiedFile(const std::string& filename, bool isSamePublicDigest);
    void addUnchangedFile(const std::string& filename);
    void addFailedFile(const std::string& filename);

    void addNewClass(const std::string& strongClassname);
    void addUnchangedClass(const std::string& strongClassname);
    void addModifiedClass(const std::string& strongClassname, bool isSamePublicDigest);

};


}

#endif
