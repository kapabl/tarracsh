#ifndef TARRACSH_STATS_H
#define TARRACSH_STATS_H

#include <string>

#include "Tarracsh.h"


namespace org::kapa::tarracsh {


struct PublicDigestResult {
    int unchangedCount{};
    int count{};
    int same{};
    int differentDigest{};
    int newFile{};
};

struct ClassfileStats {
    unsigned count{};
    unsigned parsedCount{};
    unsigned errors{};
    PublicDigestResult digest;
};


struct Results {

    ClassfileStats classfiles;

    struct {
        unsigned count{};
        unsigned parsedCount{};
        unsigned errors{};
        unsigned classfileCount{};
        PublicDigestResult digest;
        ClassfileStats classfiles;
    } jarfiles;


    unsigned long classfileTime{};
    unsigned long jarfileTime{};

    Log resultLog;


    void print(const Options& options);

};


}

#endif
