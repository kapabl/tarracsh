#include "RequestContext.h"
#include "../app/App.h"


using namespace kapa::tarracsh::server;

using kapa::tarracsh::app::server::digest::DiffRequest;

RequestContext::RequestContext()
    : _results(_options) {
    _results.log = app::App::getContext().getResults().log;
}

bool RequestContext::update(const DiffRequest& request) {
    _options.digestServer.isServerMode = true;
    _options.isPublicDigest = true;
    //_options.logFile = _context.getOptions().logFile;

    _options.input = request.input();
    _options.dryRun = request.dryrun();
    const auto result = _options.processInput();
    if (!result) {
        _errorMessage = std::format("Invalid Input: {}", _options.input);
        _results.log->writeln(_errorMessage, true);
    }

    return result;
}

