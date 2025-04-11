#if defined(WHOA_SYSTEM_WIN)

#include "common/time/Time.hpp"
#include "common/time/win/TimeManager.hpp"
#include <bc/Memory.hpp>
#include <windows.h>
#include <new>

static OsTimeManager* s_OsTimeMgr;

void OsTimeManagerCreate(TimingMethod timingMethod) {
    auto m = SMemAlloc(sizeof(OsTimeManager), __FILE__, __LINE__, 0x8);
    if (m) {
        s_OsTimeMgr = new(m) OsTimeManager(timingMethod);
    }
}

void OsTimeManagerDestroy() {
    if (s_OsTimeMgr) {
        SMemFree(s_OsTimeMgr, "delete", -1, 0x0);
        s_OsTimeMgr = nullptr;
    }
}

void OsTimeStartup(TimingMethod timingMethod) {
    OsTimeManagerCreate(timingMethod);
}

void OsTimeShutdown() {
    OsTimeManagerDestroy();
}

const char* OsTimeGetTimingMethodName(TimingMethod timingMethod) {
    switch (timingMethod) {
    case Timing_BestAvailable:
        return "[Best Available]";
    case Timing_GetTickCount:
        return "GetTickCount";
    case Timing_QueryPerformanceCounter:
        return "QueryPerformanceCounter";
    case Timing_NotSet:
        return "[Not Set]";
    default:
        return "[Unknown]";
    }
}

uint64_t OsGetAsyncTimeMsPrecise() {
    return s_OsTimeMgr->Snapshot();
}

uint64_t OsGetAsyncTimeMs() {
    return s_OsTimeMgr->Snapshot();
}

int64_t OsGetAsyncClocksPerSecond() {
    if (s_OsTimeMgr->timingMethod == Timing_QueryPerformanceCounter) {
        return s_OsTimeMgr->performanceFrequency;
    } else {
        return 1000LL;
    }
}

void OsSleep(uint32_t duration) {
    Sleep(duration);
}

#endif
