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


struct Results {

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

    Log resultLog;


    void print(const Options &options) const;
    void printAll(const Options &options) const;

};


}

#endif
