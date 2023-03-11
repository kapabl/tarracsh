#ifndef TARRACSH_SCOPED_TIMER_H
#define TARRACSH_SCOPED_TIMER_H

#include <iostream>
#include <functional>
#include <string>

#include "../../infrastructure/filesystem/Utils.h"


namespace kapa::infrastructure::profiler {

using MillisecondDuration = std::chrono::milliseconds;


class ScopedTimer {
public:
    explicit ScopedTimer(
        MillisecondDuration *pStorage = nullptr,
        const bool stdoutAtDestruction = false,
        const bool autoStart = true
        );

    void start();
    void stop();
    [[nodiscard]] std::chrono::duration<long long> getElapsedTime();
    [[nodiscard]] std::chrono::duration<long long, std::milli> getElapsedTimeMs();
    void printElapsedTime();
    ~ScopedTimer();

    [[nodiscard]] static MillisecondDuration time(const std::function<void()>& func);

    static MillisecondDuration timeWithPrint(const std::string& name, const std::function<void()>& func);

private:
    std::chrono::time_point<std::chrono::steady_clock> _startTime{};
    std::chrono::time_point<std::chrono::steady_clock> _endTime{};

    bool _coutAtDestruction{false};
    bool _stopped{false};
    MillisecondDuration *_pDurationStorage{nullptr};

};



}

#endif
