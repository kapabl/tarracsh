#ifndef TARRACSH_REQUEST_CONFIG_H
#define TARRACSH_REQUEST_CONFIG_H

#include "../app/Config.h"

namespace kapa::tarracsh::server {

class RequestConfig final : public app::Config {

public:
    RequestConfig(infrastructure::log::Log& log)
        : _results(_options), _log(log) {
    }

    domain::stats::Results &getResults() override { return _results; }
    domain::Options &getOptions() override { return _options; }
    infrastructure::log::Log &getLog() override { return _log; }
private:
    domain::Options _options;
    domain::stats::Results _results;
    infrastructure::log::Log& _log;
};


}
#endif
