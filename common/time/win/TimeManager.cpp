#if defined(WHOA_SYSTEM_WIN)

#include "common/time/win/TimeManager.hpp"
#include "common/time/Time.hpp"
#include "common/processor/Processor.hpp"
#include <windows.h>
#include <cmath>

OsTimeManager::OsTimeManager(TimingMethod tm) {
    this->timingMethod = Timing_NotSet;

    auto method = this->Calibrate();

    if (tm != Timing_BestAvailable && method != timingMethod) {
        if (tm == Timing_QueryPerformanceCounter && method == Timing_GetTickCount) {
            this->timingTestError = 5;
        }
        method = tm;
    }

    this->timingMethod = method;
    auto freq = method == Timing_QueryPerformanceCounter ? this->performanceFrequency : static_cast<int64_t>(1000LL);
    this->scaleToMs = 1000.0 / static_cast<double>(freq);
    this->timeBegin = 0;
}

TimingMethod OsTimeManager::Calibrate() {
    if (!QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&this->performanceFrequency))) {
        this->timingTestError = 1;
        return Timing_GetTickCount;
    }

    if (this->performanceFrequency == 0) {
        this->timingTestError = 2;
        return Timing_GetTickCount;
    }

    auto process = GetCurrentProcess();
    auto thread = GetCurrentThread();
    auto priorityClass = GetPriorityClass(process);
    auto threadPriority = GetThreadPriority(thread);
    SetPriorityClass(process, HIGH_PRIORITY_CLASS);
    SetThreadPriority(thread, THREAD_PRIORITY_TIME_CRITICAL);
    OsSleep(0);

    this->timingTestError = 0;

    DWORD tc1 = GetTickCount();
    DWORD tc2 = tc1;
    while (tc1 == tc2) {
        tc2 = GetTickCount();
    }

    int64_t pc1;
    int64_t pc2;

    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&pc1));

    auto nproc = OsGetProcessorCount();
    if (nproc > 1) {
        DWORD_PTR processAffinityMask;
        DWORD_PTR systemAffinityMask;
        GetProcessAffinityMask(process, &processAffinityMask, &systemAffinityMask);

        for (uint32_t i = 0; i < 512; i++) {
            SetThreadAffinityMask(thread, 1 << static_cast<DWORD_PTR>(static_cast<uint8_t>(i % nproc) & 0x1F));
            OsSleep(0);
            QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&pc2));
            if (pc2 <= pc1) {
                this->timingTestError = 4;
                break;
            }
        }

        SetThreadAffinityMask(thread, processAffinityMask);
        OsSleep(0);
    }

    if (this->timingTestError == 0) {
        auto tc3 = GetTickCount();
        while ((tc3 - tc2) < 0xFA) {
            tc3 = GetTickCount();
        }
        auto tc4 = GetTickCount();
        while (tc3 == tc4) {
            tc4 = GetTickCount();
        }
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&pc2));

        if (std::llabs(tc4 - static_cast<int64_t>(static_cast<double>(pc2 - pc1) / this->performanceFrequency * 1000.0) - tc2) >= 5) {
            this->timingTestError = 3;
        }
    }

    SetPriorityClass(process, priorityClass);
    SetThreadPriority(thread, threadPriority);
    OsSleep(0);
    return this->timingMethod == Timing_BestAvailable ? Timing_QueryPerformanceCounter : Timing_GetTickCount;
}

uint64_t OsTimeManager::Snapshot() {
    if (this->timingMethod != Timing_QueryPerformanceCounter) {
        return static_cast<uint64_t>((static_cast<double>(GetTickCount()) * this->scaleToMs) + this->timeBegin);
    }

    int64_t performanceCount;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&performanceCount));
    return static_cast<uint64_t>((static_cast<double>(performanceCount) * this->scaleToMs) + this->timeBegin);
}

#endif
