#include <gtest/gtest.h>

#include <cstdlib>
#include <filesystem>
#include <memory>
#include <string>

#include "app/server/RequestContext.h"
#include "app/server/digest/proto/Server.pb.h"
#include "domain/stats/Results.h"
#include "infrastructure/log/Log.h"

using kapa::tarracsh::app::server::digest::DiffRequest;
using kapa::tarracsh::domain::Options;
using kapa::tarracsh::domain::stats::Results;
using kapa::tarracsh::app::Context;
using kapa::infrastructure::log::Log;
using kapa::tarracsh::server::RequestContext;

namespace {

std::filesystem::path makeTempDir(const std::string &prefix) {
    auto base = std::filesystem::temp_directory_path();
    std::filesystem::path dir;
    do {
        dir = base / (prefix + std::to_string(std::rand()));
    } while (std::filesystem::exists(dir));
    std::filesystem::create_directories(dir);
    return dir;
}

class StubContext : public Context {
public:
    StubContext()
        : results(options) {
        log = std::make_shared<Log>();
        auto logPath = (std::filesystem::temp_directory_path() / "request-context-test.log").string();
        log->init(logPath);
        results.log = log;
        options.logFile = logPath;
    }

    Results &getResults() override { return results; }
    Options &getOptions() override { return options; }
    Log &getLog() override { return *log; }

    Options options;
    Results results;
    std::shared_ptr<Log> log;
};

} // namespace

TEST(RequestContextTests, UpdatePopulatesDigestOptionsFromRequest) {
    StubContext stub;
    auto tempDir = makeTempDir("request-context");

    DiffRequest request;
    request.set_input(tempDir.string());
    request.set_dryrun(true);

    RequestContext context(&stub);
    ASSERT_TRUE(context.update(request));

    const auto &options = context.getOptions();
    EXPECT_EQ(options.digest.input, tempDir.string());
    EXPECT_TRUE(options.digest.dryRun);
    EXPECT_TRUE(options.digest.isDir);
    EXPECT_TRUE(options.isPublicDigest);
    EXPECT_TRUE(options.digest.server.isServerMode);
    EXPECT_EQ(context.getResults().log, stub.log);

    std::filesystem::remove_all(tempDir);
}

TEST(RequestContextTests, UpdateFailsForInvalidInputAndSetsError) {
    StubContext stub;
    stub.log->forceStdout(true);

    DiffRequest request;
    request.set_input("/path/that/does/not/exist-" + std::to_string(std::rand()));

    RequestContext context(&stub);
    EXPECT_FALSE(context.update(request));
    const auto error = context.getErrorMessage();
    EXPECT_NE(error.find("Invalid Input"), std::string::npos);
}
