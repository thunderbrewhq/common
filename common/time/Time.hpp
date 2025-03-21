#ifndef COMMON_TIME_TIME_HPP
#define COMMON_TIME_TIME_HPP

#include <cstdint>

enum TimingMethod {
    NotSet        = -1,
    BestAvailable =  0,
    // GetTickCount (Windows), mach_absolute_time (MacOS)
    SystemMethod1 =  1,
    // QueryPerformanceCounter (Windows), Carbon Microseconds (MacOS)
    SystemMethod2 =  2
};

void OsTimeStartup(TimingMethod timingMethod);

void OsTimeShutdown();

uint64_t OsGetAsyncTimeMs();

uint64_t OsGetAsyncTimeMsPrecise();

int64_t OsGetAsyncClocksPerSecond();

void OsSleep(uint32_t duration);

#endif
