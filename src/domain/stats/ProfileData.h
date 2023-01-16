#ifndef TARRACSH_PROFILE_DATA_H
#define TARRACSH_PROFILE_DATA_H

#include <chrono>
#include "../Options.h"

namespace kapa::tarracsh::domain::stats {
struct Results;
}

namespace kapa::tarracsh::domain::stats::profiler {

using MillisecondDuration = std::chrono::milliseconds;

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
