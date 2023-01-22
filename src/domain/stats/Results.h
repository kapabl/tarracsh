#ifndef TARRACSH_STATS_H
#define TARRACSH_STATS_H

#include <string>
#include "../infrastructure/log/Log.h"
#include "../Options.h"
#include "DigestReport.h"
#include "ProfileData.h"


namespace kapa::tarracsh::domain::stats {

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

struct JarfileStats {
    Counter count{};
    Counter parsedCount{};
    Counter errors{};
    Counter classfileCount{};
    PublicDigestResult digest;
    ClassfileStats classfiles;
};



struct Results {

    Options& options;
    explicit Results(Options& options);

    void print() const;
    void forcePrint() const;
    void printAll();

    ClassfileStats standaloneClassfiles;
    JarfileStats jarfiles;
    FileTime classfileTime{};
    FileTime jarfileTime{};
    mutable std::chrono::time_point<std::chrono::steady_clock> lastPrint{std::chrono::high_resolution_clock::now()};
    std::unique_ptr<report::DigestReport> report;
    std::unique_ptr<profiler::ProfileData> profileData;
    std::shared_ptr<infrastructure::log::Log> log;

};


}

#endif
