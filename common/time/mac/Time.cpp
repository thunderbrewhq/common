#if defined(WHOA_SYSTEM_MAC)

#include "common/time/Time.hpp"
#include <mach/mach_time.h>
#include <unistd.h>

void OsTimeStartup(TimingMethod timingMethod) {
    // TODO
}

void OsTimeShutdown() {
    // TODO
}

uint64_t OsGetAsyncTimeMsPrecise() {
    static mach_timebase_info_data_t timebase;

    if (timebase.denom == 0) {
        mach_timebase_info(&timebase);
    }

    uint64_t ticks = mach_absolute_time();

    return ticks * (timebase.numer / timebase.denom) / 100000;
}

uint64_t OsGetAsyncTimeMs() {
    return OsGetAsyncTimeMsPrecise();
}

int64_t OsGetAsyncClocksPerSecond() {
    // TODO: verify
    return 1000LL;
}

void OsSleep(uint32_t duration) {
    usleep(duration);
}

#endif
