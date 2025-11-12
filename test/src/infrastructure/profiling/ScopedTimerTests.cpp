#include <gtest/gtest.h>

#include <chrono>
#include <sstream>
#include <string>
#include <thread>

#include "infrastructure/profiling/ScopedTimer.h"

using namespace kapa::infrastructure::profiler;

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

void sleepBriefly() {
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

}  // namespace

TEST(ScopedTimerTests, StopStoresDurationExactlyOnce) {
    MillisecondDuration stored{};
    ScopedTimer timer(&stored);
    sleepBriefly();
    timer.stop();
    const auto first = stored;
    sleepBriefly();
    timer.stop();  // second stop should be ignored
    EXPECT_GE(first.count(), 1);
    EXPECT_EQ(first, stored);
}

TEST(ScopedTimerTests, GetElapsedTimeUpdatesWhenNotStopped) {
    ScopedTimer timer(nullptr, false, true);
    sleepBriefly();
    const auto elapsedMs = timer.getElapsedTimeMs();
    EXPECT_GE(elapsedMs.count(), 1);
}

TEST(ScopedTimerTests, TimeUtilityReturnsDurationAndPrints) {
    ScopedStdoutCapture capture;
    const auto duration = ScopedTimer::timeWithPrint("unit-test", [] {
        sleepBriefly();
    });
    EXPECT_GE(duration.count(), 1);
    const auto output = capture.str();
    EXPECT_NE(std::string::npos, output.find("[time:unit-test("));
}

TEST(ScopedTimerTests, DestructorPrintsElapsedTimeWhenRequested) {
    ScopedStdoutCapture capture;
    {
        ScopedTimer timer(nullptr, true /*stdoutAtDestruction*/);
        sleepBriefly();
    }
    const auto output = capture.str();
    EXPECT_NE(std::string::npos, output.find("total time:"));
}
