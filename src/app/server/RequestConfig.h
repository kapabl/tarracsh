#ifndef TARRACSH_REQUEST_CONFIG_H
#define TARRACSH_REQUEST_CONFIG_H

#include "../app/Config.h"

namespace kapa::tarracsh::server {

class RequestConfig final : public app::Config {

public:
    RequestConfig(infrastructure::log::Log& log)
        : _results(_options), _log(log) {
    }

    stats::Results &getResults() override { return _results; }
    Options &getOptions() override { return _options; }
    infrastructure::log::Log &getLog() override { return _log; }
private:
    Options _options;
    stats::Results _results;
    infrastructure::log::Log& _log;
};


}
#endif
