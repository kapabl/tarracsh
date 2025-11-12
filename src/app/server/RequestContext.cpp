#include "RequestContext.h"
#include "app/AppRuntime.h"


using namespace kapa::tarracsh::server;
using kapa::tarracsh::app::server::digest::DiffRequest;
namespace runtime = kapa::tarracsh::app::runtime;

RequestContext::RequestContext(app::Context *context)
    : _results(_options),
      _appContext(context ? context : &runtime::context()) {
    _results.log = _appContext->getResults().log;
    _options.digest.server.isServerMode = true;
    _options.isPublicDigest = true;
    _options.logFile = _appContext->getOptions().logFile;
}

bool RequestContext::update(const DiffRequest &request) {
    _options.digest.input = request.input();
    _options.digest.dryRun = request.dryrun();
    _options.digest.isDiff = true;
    const auto result = _options.digest.processInput();
    if (!result) {
        _errorMessage = fmt::format("Invalid Input: {}", _options.digest.input);
        _results.log->writeln(_errorMessage, true);
    }

    return result;
}
