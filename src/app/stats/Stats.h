#ifndef TARRACSH_STATS_H
#define TARRACSH_STATS_H

#include <string>
#include "../log/Log.h"
#include "../Options.h"
#include "Report.h"
#include "ProfileData.h"


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


struct Results {

    Options& options;
    explicit Results(Options& options);

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

    log::Log log;


    void print() const;
    void forcePrint() const;
    void printAll();

    mutable std::chrono::time_point<std::chrono::steady_clock> lastPrint{std::chrono::high_resolution_clock::now()};

    std::unique_ptr <report::Report> report;
    std::unique_ptr<profiler::ProfileData> profileData;

};


}

#endif
