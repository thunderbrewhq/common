#ifndef COMMON_TIME_WIN_TIME_MANAGER_HPP
#define COMMON_TIME_WIN_TIME_MANAGER_HPP

#include "common/time/Time.hpp"

class OsTimeManager {
    public:
    double scaleToMs;
    TimingMethod timingMethod;
    uint32_t timingTestError;
    int64_t performanceFrequency;
    double timeBegin;

    OsTimeManager(TimingMethod timingMethod);
    TimingMethod Calibrate();
    uint64_t Snapshot();
};

#endif