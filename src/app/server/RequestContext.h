#ifndef TARRACSH_REQUEST_CONFIG_H
#define TARRACSH_REQUEST_CONFIG_H

#include "../app/Context.h"
#include "digest/proto/Server.pb.h"

namespace kapa::tarracsh::server {

class RequestContext final : public app::Context {

public:
    RequestContext();
    [[nodiscard]] domain::stats::Results &getResults() override { return _results; }
    [[nodiscard]] domain::Options &getOptions() override { return _options; }
    [[nodiscard]] infrastructure::log::Log &getLog() override { return *_results.log; }

    [[nodiscard]] bool update(const app::server::digest::DiffRequest& request);
    [[nodiscard]] std::string getErrorMessage() const { return _errorMessage;  };

private:
    domain::Options _options{};
    domain::stats::Results _results;
    std::vector<std::string> _errors;

    std::string _errorMessage;
    Context& _appContext;
};


}
#endif
