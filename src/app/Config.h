#ifndef TARRACSH_CONFIG_H
#define TARRACSH_CONFIG_H

#include "Options.h"
// #include "stats/Stats.h"

namespace org::kapa::tarracsh::stats {
struct Results;
}

namespace org::kapa::tarracsh::app {
class Config abstract {

public:
    virtual ~Config() = default;
    virtual stats::Results& getResults() abstract;
    virtual Options &getOptions() abstract;

};


}

#endif
