#ifndef TARRACSH_STATS_H
#define TARRACSH_STATS_H

#include <string>
#include <atomic>
#include "infrastructure/log/Log.h"
#include "domain/Options.h"
#include "Report.h"
#include "ProfileData.h"


namespace kapa::tarracsh::domain::stats {

typedef std::atomic<unsigned int> Counter;
typedef std::atomic<unsigned long> FileTime;
// typedef unsigned int Counter;
// typedef unsigned long FileTime;

struct TaskResult {
    Counter unchangedCount{};
    Counter count{};
    Counter same{};
    Counter newFile{};
    Counter differentDigest{};
};

struct CallGraphResult: TaskResult {
    
};



struct PublicDigestResult: TaskResult {

};

struct ClassfileStats {
    Counter count{};
    Counter parsedCount{};
    Counter errors{};
    TaskResult taskResult;
};

struct JarfileStats {
    Counter count{};
    Counter parsedCount{};
    Counter errors{};
    Counter classfileCount{};
    TaskResult taskResult;
    ClassfileStats classfiles;
};



struct Results {

    Options& options;
    explicit Results(Options& options);

    void print() const;
    void printProgress() const;
    void printAll() const;

    ClassfileStats standaloneClassfiles;
    JarfileStats jarfiles;
    FileTime classfileTime{};
    FileTime jarfileTime{};
    mutable std::chrono::time_point<std::chrono::steady_clock> lastPrint{std::chrono::steady_clock::now()};
//    mutable std::chrono::time_point<std::chrono::steady_clock> lastPrint = std::chrono::steady_clock::now();
    mutable uint32_t lastJarCountPrint{ 0u };
    mutable uint32_t lastClassfileCountPrint{ 0u };
    mutable bool progressStarted{ false };


    std::unique_ptr<report::Report> report;
    std::unique_ptr<profiler::ProfileData> profileData;
    std::shared_ptr<infrastructure::log::Log> log;

};


}

#endif
