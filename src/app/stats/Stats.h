#ifndef TARRACSH_STATS_H
#define TARRACSH_STATS_H

#include <string>
#include "Report.h"
#include "ProfileData.h"
#include "../Tarracsh.h"
#include "../../utils/FilesystemUtils.h"


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

    log::Log log;


    void print(const Options &options) const;
    void forcePrint(const Options &options) const;
    void printAll(const Options &options);

    mutable std::chrono::time_point<std::chrono::steady_clock> lastPrint{std::chrono::high_resolution_clock::now()};

    std::unique_ptr <report::Report> report;
    std::unique_ptr<profiler::ProfileData> profileData;

};


}

#endif
