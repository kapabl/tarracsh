#ifndef TARRACSH_SCOPED_TIMER_H
#define TARRACSH_SCOPED_TIMER_H

#include <string>

#include "ProfileData.h"
#include "../Tarracsh.h"
#include "../../infrastructure/filesystem/FilesystemUtils.h"


namespace kapa::tarracsh::stats::profiler {


class ScopedTimer {
public:
    explicit ScopedTimer(
        MillisecondDuration *pStorage = nullptr,
        const bool stdoutAtDestruction = false,
        const bool autoStart = true
        );

    void start();
    void stop();
    std::chrono::duration<long long> getElapsedTime();
    std::chrono::duration<long long, std::milli> getElapsedTimeMs();
    void printElapsedTime();
    ~ScopedTimer();

private:
    std::chrono::time_point<std::chrono::steady_clock> _startTime{};
    std::chrono::time_point<std::chrono::steady_clock> _endTime{};

    bool _coutAtDestruction{false};
    bool _stopped{false};
    MillisecondDuration *_pDurationStorage{nullptr};

};



}

#endif
