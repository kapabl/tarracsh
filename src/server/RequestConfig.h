#ifndef TARRACSH_REQUEST_CONFIG_H
#define TARRACSH_REQUEST_CONFIG_H

#include "../app/Config.h"

namespace org::kapa::tarracsh::server {

class RequestConfig final : public app::Config {

public:
    RequestConfig()
        : _results(_options) {
    }

    stats::Results &getResults() override { return _results; }
    Options &getOptions() override { return _options; }
private:
    Options _options;
    stats::Results _results;
};


}
#endif
