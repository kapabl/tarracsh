#ifndef TARRACSH_STATS_H
#define TARRACSH_STATS_H

#include <string>

#include "../app/Tarracsh.h"


namespace org::kapa::tarracsh::stats {

typedef std::atomic<unsigned int> Counter;
typedef std::atomic<unsigned long> FileTime;
// typedef unsigned int Counter;
// typedef unsigned long FileTime;

struct PublicDigestResult {
    Counter unchangedCount{};
    Counter count{};
    Counter same{};
    Counter differentDigest{};
    Counter newFile{};
};

struct ClassfileStats {
    Counter count{};
    Counter parsedCount{};
    Counter errors{};
    PublicDigestResult digest;
};

struct Results;

struct JarResult {
    std::string filename;
    bool isNew{ false };
    bool isModified{ false };
    bool isSamePublicDigest{ true };
};

struct ClassfileResult {
    std::string filename;
    std::string classname;
    bool isNew{ false };
    bool isModified{ false };
    bool isSamePublicDigest{ false };
};

struct Report {
    Results& results;
    std::vector<JarResult> jarResults;
    std::vector<ClassfileResult> classfileResults;

    std::mutex mutex;
    explicit Report(Results& results) : results(results) {}
    void asNew(const std::string& filename);
    void asModified(const std::string& filename, bool isSamePublicDigest );
    void asUnchanged(const std::string& filename);

    void asNewClass(const std::string& filename);
    void asModifiedClass(const std::string& fullClassname, bool isSamePublicDigest);
    void asUnchangedClass(const std::string& filename);

    void print();
};


struct Results {

    Results();

    ClassfileStats classfiles;

    struct {
        Counter count{};
        Counter parsedCount{};
        Counter errors{};
        Counter classfileCount{};
        PublicDigestResult digest;
        ClassfileStats classfiles;
    } jarfiles;


    FileTime classfileTime{};
    FileTime jarfileTime{};

    Log log;


    void print(const Options &options) const;
    void printAll(const Options &options) const;

    mutable std::chrono::time_point<std::chrono::steady_clock> lastPrint{ std::chrono::high_resolution_clock::now() };

    Report report;

};



}

#endif
