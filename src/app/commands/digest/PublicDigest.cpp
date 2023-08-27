#include "PublicDigest.h"
#include "app/App.h"
#include "app/Analyzer.h"
#include "DigestAnalyzer.h"
#include "app/commands/Query.h"
#include "app/server/digest/ServerCommand.h"
#include "infrastructure/profiling/ScopedTimer.h"

using kapa::tarracsh::app::server::digest::ServerCommand;
using kapa::tarracsh::app::commands::Query;
using kapa::tarracsh::domain::ServerOptions;
using kapa::tarracsh::domain::ClientOptions;
using kapa::infrastructure::app::cli::ExitCode;
using kapa::infrastructure::profiler::ScopedTimer;

using namespace kapa::tarracsh::app::commands::digest;


PublicDigest::PublicDigest(CLI::App *parent)
        : DbBasedCommand(parent, App::getGlobalOptions().digest) {
}

bool PublicDigest::initDb() {

    ScopedTimer timer(&_results.profileData->initDb);


    _db = domain::db::digest::DigestDb::create({_options.outputDir, _results.log.get()}, _options.digest.rebuild,
                                               false);
    const auto result = _db.get() != nullptr;
    return result;

}

bool PublicDigest::runAsStandalone() {
    const auto result = initDb();
    if (result) {
        DigestAnalyzer analyzer(App::getContext(), _db);
        analyzer.runWithPrint();
    } else {
        _results.log->writeln("Error initializing Db", true);
    }
    return result;
}

ExitCode PublicDigest::digestInput() {
    ExitCode result = 0;

    ScopedTimer::timeWithPrint(
            "digest-input",
            [this, &result]() -> void {
                if (_options.digest.isValidInput()) {
                    if (isClientMode()) {
                        result = runAsClient() ? 0 : 1;
                    } else {
                        result = runAsStandalone() ? 0 : 1;
                    }

                    if (result == 0 && _options.digest.isDiff) {
                        _results.report->print();
                    }
                } else {
                    result = 1;
                }
            });

    return result;
}

bool PublicDigest::runAsServer() {
    const auto result = ServerCommand::run(App::getContext());
    return result;
}

ExitCode PublicDigest::run() {
    ExitCode result = 0;
    _options.digest.server.isServerMode = _subCommand->got_subcommand(_serverSubCommand);
    if (isServerMode()) {
        result = runAsServer();
    } else if (!_options.digest.queryValue.empty()) {
        result = Query::run(App::getContext());
    } else {
        result = digestInput();
    }
    return result;

}

bool PublicDigest::runAsClient() {
    const auto result = ServerCommand::run(App::getContext());
    return result;
}

void PublicDigest::addMainSubCommand() {
    _subCommand = _parent->add_subcommand("public-digest", "Public digest of jar files and classfiles");
    _subCommand->add_flag("--verbose, -v", _options.verbose, "Verbose output");
}
