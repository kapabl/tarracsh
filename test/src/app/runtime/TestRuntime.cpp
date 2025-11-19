#include "src/app/AppRuntime.h"
#include "test/src/app/runtime/TestRuntime.h"

#include <memory>
#include <stdexcept>

#include "domain/Options.h"
#include "domain/stats/Results.h"
#include "infrastructure/log/Log.h"

namespace {

class StubRuntimeContext final : public kapa::tarracsh::app::Context {
public:
    StubRuntimeContext() {
        reset(nullptr);
    }

    void reset(const std::shared_ptr<kapa::infrastructure::log::Log> &log) {
        _options = kapa::tarracsh::domain::Options{};
        _results = std::make_unique<kapa::tarracsh::domain::stats::Results>(_options);
        _log = log ? log : std::make_shared<kapa::infrastructure::log::Log>();
        _results->log = _log;
    }

    kapa::tarracsh::domain::stats::Results &getResults() override { return *_results; }
    kapa::tarracsh::domain::Options &getOptions() override { return _options; }
    kapa::infrastructure::log::Log &getLog() override { return *_log; }

private:
    kapa::tarracsh::domain::Options _options;
    std::unique_ptr<kapa::tarracsh::domain::stats::Results> _results;
    std::shared_ptr<kapa::infrastructure::log::Log> _log;
};

StubRuntimeContext &stub() {
    static StubRuntimeContext context;
    return context;
}

} // namespace

namespace kapa::tarracsh::app::runtime {

domain::stats::Results &global_results() {
    return stub().getResults();
}

domain::Options &global_options() {
    return stub().getOptions();
}

Context &context() {
    return stub();
}

TarracshCli &app() {
    throw std::logic_error("Runtime app() stub should not be called in tests.");
}

} // namespace kapa::tarracsh::app::runtime

namespace kapa::tarracsh::app::runtime::test {

void reset(const std::shared_ptr<kapa::infrastructure::log::Log> &log) {
    stub().reset(log);
}

kapa::tarracsh::domain::Options &options() {
    return stub().getOptions();
}

kapa::tarracsh::domain::stats::Results &results() {
    return stub().getResults();
}

} // namespace kapa::tarracsh::app::runtime::test
