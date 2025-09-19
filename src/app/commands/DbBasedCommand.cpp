#include "DbBasedCommand.h"
#include "app/App.h"
#include "app/Analyzer.h"
#include "app/commands/Query.h"
#include "app/server/digest/ServerCommand.h"
#include "infrastructure/profiling/ScopedTimer.h"

using kapa::tarracsh::app::server::digest::ServerCommand;
using kapa::tarracsh::app::commands::Query;
using kapa::tarracsh::domain::ServerOptions;
using kapa::tarracsh::domain::ClientOptions;
using kapa::infrastructure::app::cli::ExitCode;
using kapa::infrastructure::profiler::ScopedTimer;

using namespace kapa::tarracsh::app::commands;


DbBasedCommand::DbBasedCommand(CLI::App *parent, domain::SubCommandOptions &dbBasedOptions)
    : Command(parent),
      _results(App::getGlobalResults()),
      _options(App::getGlobalOptions()),
      _dbBasedOptions(dbBasedOptions) {
}

bool DbBasedCommand::isServerMode() const {
    const auto result = _dbBasedOptions.server.isServerMode;
    return result;
}

bool DbBasedCommand::runAsServer() {

    //TODO fix when server mode is implemented for call-graph
    const auto result = ServerCommand::run(App::getContext());
    return result;
}

CLI::App *DbBasedCommand::addServerSubCommand() const {
    const auto result = _subCommand->add_subcommand("server", "Server commands - Default start server");

    ServerOptions &serverOptions = _dbBasedOptions.server;

    const auto port = result->add_option("--port", serverOptions.port, "Server Port")->default_val(serverOptions.port);
    const auto listenAddress = result->add_option("--listen-addr", serverOptions.listenAddress, "Listen address")->
                                       default_val("0.0.0.0");

    const auto stop = result->add_flag("--stop", serverOptions.stopServer, "Stop Server")->excludes(listenAddress)->
                              excludes(port);

    return result;
}

CLI::Option *DbBasedCommand::addClientOptions() const {

    ClientOptions &clientOptions = _dbBasedOptions.client;

    const auto diffOnServer = _subCommand->add_flag("--server", clientOptions.isClientMode,
                                                    "Run Diff against server")
                                         ->default_val(false);

    const auto port = _subCommand->add_option("--port", clientOptions.port, "Server Port")->default_val(
        clientOptions.port);
    const auto listenAddress = _subCommand->add_option("--host", clientOptions.host, "Server host name")
                                          ->default_val(clientOptions.host);

    return diffOnServer;

}

bool DbBasedCommand::isClientMode() const {
    const auto result =_dbBasedOptions.client.isClientMode;
    return result;
}

bool DbBasedCommand::runAsClient() {
    const auto result = ServerCommand::run(App::getContext());
    return result;
}

CLI::Option *DbBasedCommand::addQueryOptions() const {
    const auto result = _subCommand->add_option(
        "--query",
        _dbBasedOptions.queryValue,
        "Run a database query; see README → Query language. Combine with --display-raw to print raw values.");
    _subCommand->add_flag("--display-raw",_dbBasedOptions.displayRaw, "Display raw column values")
               ->needs(result);
    return result;
}

void DbBasedCommand::addCommand() {

    addMainSubCommand();

    const auto input = _subCommand->add_option("--input,-i",_dbBasedOptions.input,
                                               "Input: directory, jar file or class file");

    const auto rebuild = _subCommand->add_flag("--rebuild",_dbBasedOptions.rebuild, "Rebuild Db")
                                    ->needs(input);

    const auto dryRun = _subCommand->add_flag("--dry-run",_dbBasedOptions.dryRun,
                                              "Check Against Db, default behavior is check and add/update")
                                   ->needs(input);

    rebuild->excludes(dryRun);

    const auto diff = _subCommand->add_flag("--diff",_dbBasedOptions.isDiff, "Create Diff report")
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
