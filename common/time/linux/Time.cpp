#if defined(WHOA_SYSTEM_LINUX)

#include "common/time/Time.hpp"

#include <chrono>
#include <unistd.h>

void OsTimeStartup(TimingMethod timingMethod) {
    // TODO
}

void OsTimeShutdown() {
    // TODO
}

uint64_t OsGetAsyncTimeMsPrecise() {
    auto now = std::chrono::steady_clock::now();
    uint64_t ticks = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    return ticks;
}

uint64_t OsGetAsyncTimeMs() {
    return OsGetAsyncTimeMsPrecise();
}

void OsSleep(uint32_t duration) {
    usleep(duration);
}

#endif