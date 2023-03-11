#include "ScopedTimer.h"

#include <iostream>


using namespace kapa::infrastructure::profiler;

ScopedTimer::ScopedTimer(MillisecondDuration *pStorage, const bool stdoutAtDestruction,
                         const bool autoStart)
    : _coutAtDestruction(stdoutAtDestruction),
      _pDurationStorage(pStorage) {

    if (autoStart) {
        start();
    }
}

void ScopedTimer::start() {
    _startTime = std::chrono::high_resolution_clock::now();
}

void ScopedTimer::stop() {
    if (!_stopped) {
        _stopped = true;
        _endTime = std::chrono::high_resolution_clock::now();
        if (_pDurationStorage != nullptr) {
            const auto value = _endTime - _startTime;
            *_pDurationStorage = std::chrono::duration_cast<std::chrono::milliseconds>(value);
        }
    }
}

std::chrono::duration<long long> ScopedTimer::getElapsedTime() {
    if (!_stopped) {
        _endTime = std::chrono::high_resolution_clock::now();
    }
    const auto result = std::chrono::duration_cast<std::chrono::seconds>(_endTime - _startTime);
    return result;
}

std::chrono::duration<long long, std::milli> ScopedTimer::getElapsedTimeMs() {
    if (!_stopped) {
        _endTime = std::chrono::high_resolution_clock::now();
    }
    const auto result = std::chrono::duration_cast<std::chrono::milliseconds>(_endTime - _startTime);
    return result;
}

void ScopedTimer::printElapsedTime() {
    std::cout << std::endl << std::format("total time: {}", getElapsedTimeMs()) << std::endl;
}

ScopedTimer::~ScopedTimer() {
    stop();
    if (_coutAtDestruction) {
        printElapsedTime();
    }
}

MillisecondDuration ScopedTimer::time(const std::function<void()> &func) {
    MillisecondDuration result;
    {
        ScopedTimer timer(&result);
        func();
    }
    return result;
}

MillisecondDuration ScopedTimer::timeWithPrint(const std::string &name, const std::function<void()> &func) {
    const auto result = time(func);
    std::cout << std::format("[time:{}({})]", name, result) << std::endl;
    return result;
}
