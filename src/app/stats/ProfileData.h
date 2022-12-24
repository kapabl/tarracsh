#ifndef TARRACSH_PROFILE_DATA_H
#define TARRACSH_PROFILE_DATA_H

#include <chrono>
#include "../app/Tarracsh.h"

namespace org::kapa::tarracsh::stats {
struct Results;
}

namespace org::kapa::tarracsh::stats::profiler {

// using MillisecondDuration = std::chrono::duration<long long, std::milli>;
using MillisecondDuration = std::chrono::milliseconds;
// using MillisecondDuration = long long;

struct ProfileData {
    Results &results;
    MillisecondDuration analyzerTime{0};
    MillisecondDuration initAnalyzer{0};
    MillisecondDuration initDb{0};
    MillisecondDuration writeDigestDb{0};
    MillisecondDuration writeCallGraphDb{0};

    explicit ProfileData(Results &results);

    void output(const Options &options) const;
};


}

#endif
