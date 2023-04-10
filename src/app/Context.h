#ifndef TARRACSH_CONFIG_H
#define TARRACSH_CONFIG_H

#include "domain/Options.h"
#include "domain/stats/Results.h"
#include "infrastructure/log/Log.h"

namespace kapa::tarracsh::app {
class Context {

public:
    virtual ~Context() = default;
    virtual domain::stats::Results& getResults() = 0;
    virtual domain::Options &getOptions() = 0;
    virtual infrastructure::log::Log &getLog() = 0;

};


}

#endif
