#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "infrastructure/app/CliApp.h"

using kapa::infrastructure::app::cli::CliApp;

namespace {

std::vector<char *> makeArgv(std::vector<std::string> &args) {
    std::vector<char *> argv;
    argv.reserve(args.size());
    for (auto &arg : args) {
        argv.push_back(arg.data());
    }
    return argv;
}

}  // namespace

TEST(CliApp, ParsesFlagsAndOptions) {
    CliApp app("Tarracsh CLI", "tarracsh");

    bool verbose = false;
    int workers = 0;
    app.add_flag("--verbose", verbose);
    app.add_option("--workers", workers)->default_val(4);

    std::vector<std::string> args = {"tarracsh", "--verbose", "--workers", "8"};
    auto argv = makeArgv(args);

    EXPECT_NO_THROW(app.parse(static_cast<int>(args.size()), argv.data()));
    EXPECT_TRUE(verbose);
    EXPECT_EQ(8, workers);
    EXPECT_EQ("Tarracsh CLI", app.get_description());
    EXPECT_EQ("tarracsh", app.get_name());
}

TEST(CliApp, PropagatesValidationErrors) {
    CliApp app("Tarracsh CLI", "tarracsh");
    int workers = 0;
    app.add_option("--workers", workers)->check(CLI::Range(1, 16));

    std::vector<std::string> args = {"tarracsh", "--workers", "0"};
    auto argv = makeArgv(args);

    EXPECT_THROW(app.parse(static_cast<int>(args.size()), argv.data()), CLI::ParseError);
    EXPECT_EQ(0, workers);
}
