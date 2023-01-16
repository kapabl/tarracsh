#include "PublicDigest.h"
#include "../Analyzer.h"
#include "../domain/db/Query.h"
#include "../server/digest/ServerCommand.h"


using namespace kapa::infrastructure::app::cli::command;
using namespace kapa::tarracsh;
using namespace app;
using namespace commands;


PublicDigest::PublicDigest(CLI::App* parent)
    : Command(parent), _results(App::getGlobalResults()), _options(App::getGlobalOptions()) {
}

ExitCode PublicDigest::run() const {
    ExitCode result = 0;
    _options.digestServer.enabled = _parent->got_subcommand(_digestServerOptions);
    if (_options.digestServer.enabled) {
        server::digest::ServerCommand::run(App::getApp());
    } else if (!_options.queryValue.empty()) {
        db::query::QueryCommand::run(App::getApp());
    } else {
        if (App::isValidInput(_options)) {
            Analyzer analyzer(App::getApp());
            analyzer.run();
        } else {
            result = 1;
        }
    }
    return result;

}

CLI::App *PublicDigest::addServerOptions() const {
    const auto result = _subCommand->add_subcommand("server", "Server commands - Default start server");

    ServerOptions& serverOptions = _options.digestServer;

    result->add_option("--port", serverOptions.port, "Server Port")->default_val(serverOptions.port);
    const auto listenAddress = result->add_option("--listen-addr", serverOptions.listenAddress, "Listen address")->
                                       default_val("0.0.0.0");

    const auto stop = result->add_flag("--stop", serverOptions.stopServer, "Stop Server")->excludes(listenAddress);
    result->add_flag("--host", serverOptions.host, "Server Host")
          ->excludes(listenAddress)
          ->default_val("localhost");

    return result;
}

void PublicDigest::addCommand() {

    _subCommand = _parent->add_subcommand("public-digest", "Public digest of jar files and classfiles");
    const auto input = _subCommand->add_option("--input,-i", _options.input, "Input: directory, jar file or class file");

    const auto rebuild = _subCommand->add_flag("--rebuild", _options.rebuild, "Rebuild Digest Db")
                               ->needs(input);

    const auto dryRun = _subCommand->add_flag("--dry-run", _options.dryRun,
                                         "Check Against Digest Db, default behavior is check and add/update")
                              ->needs(input);

    rebuild->excludes(dryRun);

    const auto diff = _subCommand->add_flag("--diff", _options.doDiffReport, "Create Diff report")
                            ->needs(input);
    _subCommand->add_flag("--print-diff", _options.printDiffReport,
                     "Print Diff report to stdout")->needs(diff)
          ->needs(input);

    _subCommand->add_flag("--verbose, -v", _options.verbose, "Verbose output");

    const auto query = _subCommand->add_option("--query", _options.queryValue, "TODO Query Help - schema")
                             ->needs(input);

    _digestServerOptions = addServerOptions();
    _digestServerOptions->excludes(input);

}
