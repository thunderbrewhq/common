#ifndef COMMON_TIME_TIME_HPP
#define COMMON_TIME_TIME_HPP

#include <cstdint>

enum TimingMethod {
    Timing_NotSet                  = -1,
    Timing_BestAvailable           =  0,
    Timing_GetTickCount            =  1,
    Timing_QueryPerformanceCounter =  2
};

void OsTimeStartup(TimingMethod timingMethod);

void OsTimeShutdown();

uint64_t OsGetAsyncTimeMs();

uint64_t OsGetAsyncTimeMsPrecise();

int64_t OsGetAsyncClocksPerSecond();

void OsSleep(uint32_t duration);

#if defined(WHOA_SYSTEM_WIN)

const char* OsTimeGetTimingMethodName(TimingMethod timingMethod);

#endif

#endif
