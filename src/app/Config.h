#ifndef TARRACSH_CONFIG_H
#define TARRACSH_CONFIG_H

#include "Options.h"
#include "../infrastructure/log/Log.h"

namespace kapa::tarracsh::app::stats {
struct Results;
}

namespace kapa::tarracsh::app {
class Config {

public:
    virtual ~Config() = default;
    virtual stats::Results& getResults() = 0;
    virtual Options &getOptions() = 0;
    virtual infrastructure::log::Log &getLog() = 0;

};


}

#endif
