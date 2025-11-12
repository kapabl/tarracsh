#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>

#include "infrastructure/log/Log.h"

using namespace kapa::infrastructure::log;

namespace {

class ScopedStdoutCapture {
public:
    ScopedStdoutCapture()
        : _oldBuf(std::cout.rdbuf(_buffer.rdbuf())) {}
    ScopedStdoutCapture(const ScopedStdoutCapture &) = delete;
    ScopedStdoutCapture &operator=(const ScopedStdoutCapture &) = delete;
    ~ScopedStdoutCapture() { std::cout.rdbuf(_oldBuf); }

    [[nodiscard]] std::string str() const { return _buffer.str(); }

private:
    std::ostringstream _buffer;
    std::streambuf *_oldBuf{nullptr};
};

std::filesystem::path makeTempFile(const std::string &prefix) {
    auto dir = std::filesystem::temp_directory_path() / prefix;
    std::filesystem::create_directories(dir);
    return dir / "log.txt";
}

std::string readFile(const std::filesystem::path &path) {
    std::ifstream file(path);
    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

}  // namespace

TEST(LogTests, WriteAndWritelnEmitToFileAndStdout) {
    Log log;
    const auto logFile = makeTempFile("log-tests-write");
    log.init(logFile.string());

    ScopedStdoutCapture capture;
    log.writeln("first line", true);
    log.write("second line", true);

    const auto fileContent = readFile(logFile);
    EXPECT_EQ("first line\nsecond line", fileContent);
    const auto stdoutContent = capture.str();
    EXPECT_NE(std::string::npos, stdoutContent.find("first line"));
    EXPECT_NE(std::string::npos, stdoutContent.find("second line"));
}

TEST(LogTests, ForceStdoutOverridesWriteFlag) {
    Log log;
    const auto logFile = makeTempFile("log-tests-force");
    log.init(logFile.string());
    log.forceStdout(true);

    ScopedStdoutCapture capture;
    log.write("forced output", false);

    const auto stdoutContent = capture.str();
    EXPECT_NE(std::string::npos, stdoutContent.find("forced output"));
    const auto fileContent = readFile(logFile);
    EXPECT_EQ("forced output", fileContent);
}

TEST(LogTests, ColorHelpersWrapOutputAndTruncateFile) {
    const auto logFile = makeTempFile("log-tests-color");
    std::ofstream(logFile) << "stale data";
    Log::emptyLogFile(logFile.string());
    EXPECT_EQ(0u, std::filesystem::file_size(logFile));

    Log log;
    log.init(logFile.string());

    ScopedStdoutCapture capture;
    log.writelnRed("error");
    log.writeGreen("ok");

    const auto stdoutContent = capture.str();
    EXPECT_NE(std::string::npos, stdoutContent.find("\033[31merror"));
    EXPECT_NE(std::string::npos, stdoutContent.find("\033[32mok"));

    const auto fileContent = readFile(logFile);
    EXPECT_EQ("error\nok", fileContent);
}
