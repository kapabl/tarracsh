#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

#include <CLI/CLI.hpp>

#include "app/commands/DbBasedCommand.h"
#include "domain/Options.h"

using namespace kapa::tarracsh;
using namespace app::commands;
using kapa::tarracsh::domain::SubCommandOptions;
using kapa::tarracsh::domain::Options;
using kapa::infrastructure::app::cli::ExitCode;

namespace {

class DummyDbCommand : public DbBasedCommand {
public:
    DummyDbCommand(CLI::App *parent, SubCommandOptions &options)
        : DbBasedCommand(parent, options) {}

    void addMainSubCommand() override {
        _subCommand = _parent->add_subcommand("dummy", "Dummy command for testing");
    }

    bool initDb() override { initDbCalled = true; return initDbResult; }
    bool runAsStandalone() override { runStandaloneCalled = true; return standaloneResult; }
    ExitCode run() override { return 0; }

    CLI::App *serverCommand() const { return _serverSubCommand; }

    bool initDbCalled{false};
    bool runStandaloneCalled{false};
    bool initDbResult{true};
    bool standaloneResult{true};
};

std::vector<char *> makeArgv(std::vector<std::string> &args) {
    std::vector<char *> argv;
    argv.reserve(args.size());
    for (auto &arg : args) {
        argv.push_back(arg.data());
    }
    return argv;
}

} // namespace

TEST(DbBasedCommandTests, QueryOptionsPopulateSubCommandOptions) {
    CLI::App root{"test"};
    Options options;
    auto &subOptions = options.parse;
    DummyDbCommand command(&root, subOptions);
    command.addCommand();

    std::vector<std::string> args{"prog", "dummy", "--query", "list files", "--display-raw"};
    auto argv = makeArgv(args);
    ASSERT_NO_THROW(root.parse(static_cast<int>(argv.size()), argv.data()));

    EXPECT_EQ(subOptions.queryValue, "list files");
    EXPECT_TRUE(subOptions.displayRaw);
}

TEST(DbBasedCommandTests, ServerSubcommandSetsServerModeAndPort) {
    CLI::App root{"test"};
    Options options;
    auto &subOptions = options.digest;
    DummyDbCommand command(&root, subOptions);
    command.addCommand();

    std::vector<std::string> args{
        "prog",
        "dummy",
        "server",
        "--port",
        "12345",
        "--listen-addr",
        "0.0.0.0"
    };
    auto argv = makeArgv(args);
    ASSERT_NO_THROW(root.parse(static_cast<int>(argv.size()), argv.data()));

    EXPECT_TRUE(command.serverCommand()->parsed());
    EXPECT_EQ(subOptions.server.port, 12345);
    EXPECT_EQ(subOptions.server.listenAddress, "0.0.0.0");
}

TEST(DbBasedCommandTests, ClientFlagEnablesClientMode) {
    CLI::App root{"test"};
    Options options;
    auto &subOptions = options.digest;
    DummyDbCommand command(&root, subOptions);
    command.addCommand();

    const auto inputPath = "/tmp/dummy-input.jar";
    std::vector<std::string> args{
        "prog",
        "dummy",
        "--diff",
        "--input",
        inputPath,
        "--server",
        "--port",
        "5555",
        "--host",
        "localhost"
    };
    auto argv = makeArgv(args);
    ASSERT_NO_THROW(root.parse(static_cast<int>(argv.size()), argv.data()));

    EXPECT_TRUE(subOptions.client.isClientMode);
    EXPECT_EQ(subOptions.client.port, 5555);
    EXPECT_EQ(subOptions.client.host, "localhost");
}

TEST(DbBasedCommandTests, DiffFlagRequiresInput) {
    CLI::App root{"test"};
    Options options;
    auto &subOptions = options.digest;
    DummyDbCommand command(&root, subOptions);
    command.addCommand();

    std::vector<std::string> args{"prog", "dummy", "--diff"};
    auto argv = makeArgv(args);
    EXPECT_THROW(root.parse(static_cast<int>(argv.size()), argv.data()), CLI::ParseError);
}

TEST(DbBasedCommandTests, QueryExcludesInputOption) {
    CLI::App root{"test"};
    Options options;
    auto &subOptions = options.digest;
    DummyDbCommand command(&root, subOptions);
    command.addCommand();

    std::vector<std::string> args{
        "prog",
        "dummy",
        "--input",
        "/tmp/input",
        "--query",
        "list files"
    };
    auto argv = makeArgv(args);
    EXPECT_THROW(root.parse(static_cast<int>(argv.size()), argv.data()), CLI::ParseError);
}

TEST(DbBasedCommandTests, ServerSubcommandCannotCombineWithInput) {
    CLI::App root{"test"};
    Options options;
    auto &subOptions = options.digest;
    DummyDbCommand command(&root, subOptions);
    command.addCommand();

    std::vector<std::string> args{
        "prog",
        "dummy",
        "server",
        "--input",
        "/tmp/input"
    };
    auto argv = makeArgv(args);
    EXPECT_THROW(root.parse(static_cast<int>(argv.size()), argv.data()), CLI::ParseError);
}

TEST(DbBasedCommandTests, ServerFlagRequiresDiffOption) {
    CLI::App root{"test"};
    Options options;
    auto &subOptions = options.digest;
    DummyDbCommand command(&root, subOptions);
    command.addCommand();

    std::vector<std::string> args{
        "prog",
        "dummy",
        "--input",
        "/tmp/input",
        "--server"
    };
    auto argv = makeArgv(args);
    EXPECT_THROW(root.parse(static_cast<int>(argv.size()), argv.data()), CLI::ParseError);
}

TEST(DbBasedCommandTests, DisplayRawFlagRequiresQuery) {
    CLI::App root{"test"};
    Options options;
    auto &subOptions = options.digest;
    DummyDbCommand command(&root, subOptions);
    command.addCommand();

    std::vector<std::string> args{
        "prog",
        "dummy",
        "--display-raw"
    };
    auto argv = makeArgv(args);
    EXPECT_THROW(root.parse(static_cast<int>(argv.size()), argv.data()), CLI::ParseError);
}

TEST(DbBasedCommandTests, RebuildFlagExcludesDryRun) {
    CLI::App root{"test"};
    Options options;
    auto &subOptions = options.digest;
    DummyDbCommand command(&root, subOptions);
    command.addCommand();

    std::vector<std::string> args{
        "prog",
        "dummy",
        "--input",
        "/tmp/input",
        "--rebuild",
        "--dry-run"
    };
    auto argv = makeArgv(args);
    EXPECT_THROW(root.parse(static_cast<int>(argv.size()), argv.data()), CLI::ParseError);
}

TEST(DbBasedCommandTests, ServerStopCannotCombineWithPortOrAddress) {
    CLI::App root{"test"};
    Options options;
    auto &subOptions = options.digest;
    DummyDbCommand command(&root, subOptions);
    command.addCommand();

    std::vector<std::string> args{
        "prog",
        "dummy",
        "server",
        "--stop",
        "--port",
        "5000"
    };
    auto argv = makeArgv(args);
    EXPECT_THROW(root.parse(static_cast<int>(argv.size()), argv.data()), CLI::ParseError);

    std::vector<std::string> addrArgs{
        "prog",
        "dummy",
        "server",
        "--stop",
        "--listen-addr",
        "127.0.0.1"
    };
    auto addrArgv = makeArgv(addrArgs);
    EXPECT_THROW(root.parse(static_cast<int>(addrArgv.size()), addrArgv.data()), CLI::ParseError);
}

TEST(DbBasedCommandTests, DryRunFlagRequiresInput) {
    CLI::App root{"test"};
    Options options;
    auto &subOptions = options.digest;
    DummyDbCommand command(&root, subOptions);
    command.addCommand();

    std::vector<std::string> args{
        "prog",
        "dummy",
        "--dry-run"
    };
    auto argv = makeArgv(args);
    EXPECT_THROW(root.parse(static_cast<int>(argv.size()), argv.data()), CLI::ParseError);
}

TEST(DbBasedCommandTests, RebuildFlagRequiresInput) {
    CLI::App root{"test"};
    Options options;
    auto &subOptions = options.digest;
    DummyDbCommand command(&root, subOptions);
    command.addCommand();

    std::vector<std::string> args{
        "prog",
        "dummy",
        "--rebuild"
    };
    auto argv = makeArgv(args);
    EXPECT_THROW(root.parse(static_cast<int>(argv.size()), argv.data()), CLI::ParseError);
}

TEST(DbBasedCommandTests, ServerFlagRequiresInputEvenWhenDiffProvided) {
    CLI::App root{"test"};
    Options options;
    auto &subOptions = options.digest;
    DummyDbCommand command(&root, subOptions);
    command.addCommand();

    std::vector<std::string> args{
        "prog",
        "dummy",
        "--diff",
        "--server"
    };
    auto argv = makeArgv(args);
    EXPECT_THROW(root.parse(static_cast<int>(argv.size()), argv.data()), CLI::ParseError);
}
