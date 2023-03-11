#include "PublicDigest.h"
#include "../../App.h"
#include "../../Analyzer.h"
#include "QueryCommand.h"
#include "../../server/digest/ServerCommand.h"
#include "../../../infrastructure/profiling/ScopedTimer.h"

using kapa::tarracsh::app::server::digest::ServerCommand;
using kapa::tarracsh::app::commands::digest::QueryCommand;
using kapa::tarracsh::domain::DigestServerOptions;
using kapa::tarracsh::domain::DigestClientOptions;
using kapa::infrastructure::app::cli::ExitCode;
using kapa::infrastructure::profiler::ScopedTimer;

using namespace kapa::tarracsh::app::commands::digest;


PublicDigest::PublicDigest(CLI::App *parent)
    : Command(parent), _results(App::getGlobalResults()), _options(App::getGlobalOptions()) {
}

bool PublicDigest::initDb() {

    ScopedTimer timer(&_results.profileData->initDb);


    _db = domain::db::digest::DigestDb::create({ _options.outputDir, _results.log.get()}, _options.digest.rebuild, false);
    const auto result = _db.get() != nullptr;
    return result;

}

bool PublicDigest::runAsStandalone() {
    const auto result = initDb();
    if (result) {
        Analyzer analyzer(App::getContext(), _db);
        analyzer.runWithPrint();
    } else {
        _results.log->writeln("Error initializing digest Db", true);
    }
    return result;
}

ExitCode PublicDigest::digestInput() {
    ExitCode result = 0;

    ScopedTimer::timeWithPrint(
        "digest-input",
        [this, &result]()-> void {
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

bool PublicDigest::isServerMode() const {
    const auto result = _options.digest.server.isServerMode;
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
        result = QueryCommand::run(App::getContext());
    } else {
        result = digestInput();
    }
    return result;

}

CLI::App *PublicDigest::addServerSubCommand() const {
    const auto result = _subCommand->add_subcommand("server", "Server commands - Default start server");

    DigestServerOptions &serverOptions = _options.digest.server;

    const auto port = result->add_option("--port", serverOptions.port, "Server Port")->default_val(serverOptions.port);
    const auto listenAddress = result->add_option("--listen-addr", serverOptions.listenAddress, "Listen address")->
                                       default_val("0.0.0.0");

    const auto stop = result->add_flag("--stop", serverOptions.stopServer, "Stop Server")->excludes(listenAddress)->
                              excludes(port);

    return result;
}

CLI::Option *PublicDigest::addClientOptions() const {

    DigestClientOptions &clientOptions = _options.digest.client;

    const auto diffOnServer = _subCommand->add_flag("--server", clientOptions.isClientMode,
                                                    "Run Diff against server")
                                         ->default_val(false);

    const auto port = _subCommand->add_option("--port", clientOptions.port, "Server Port")->default_val(
        clientOptions.port);
    const auto listenAddress = _subCommand->add_option("--host", clientOptions.host, "Server host name")
                                          ->default_val(clientOptions.host);

    return diffOnServer;

}

bool PublicDigest::isClientMode() const {
    const auto result = _options.digest.client.isClientMode;
    return result;
}

bool PublicDigest::runAsClient() {
    const auto result = ServerCommand::run(App::getContext());
    return result;
}

CLI::Option *PublicDigest::addQueryOptions() const {
    const auto result = _subCommand->add_option("--query", _options.digest.queryValue, "TODO Query Help - schema");
    _subCommand->add_flag("--display-raw", _options.digest.displayRaw, "Display row value of columns")
        ->needs(result);
    return result;
}

void PublicDigest::addCommand() {

    _subCommand = _parent->add_subcommand("public-digest", "Public digest of jar files and classfiles");
    _subCommand->add_flag("--verbose, -v", _options.verbose, "Verbose output");

    const auto input = _subCommand->add_option("--input,-i", _options.digest.input,
                                               "Input: directory, jar file or class file");

    const auto rebuild = _subCommand->add_flag("--rebuild", _options.digest.rebuild, "Rebuild Digest Db")
                                    ->needs(input);

    const auto dryRun = _subCommand->add_flag("--dry-run", _options.digest.dryRun,
                                              "Check Against Digest Db, default behavior is check and add/update")
                                   ->needs(input);

    rebuild->excludes(dryRun);

    const auto diff = _subCommand->add_flag("--diff", _options.digest.isDiff, "Create Diff report")
                                 ->needs(input);

    const auto clientOptions = addClientOptions();
    clientOptions
        ->needs(diff)
        ->needs(input);

    const auto query = addQueryOptions();
    query->excludes(input);

    _serverSubCommand = addServerSubCommand();
    _serverSubCommand->excludes(input);

}
