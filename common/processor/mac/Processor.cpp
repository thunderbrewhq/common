#if defined(WHOA_SYSTEM_MAC)

#include "common/processor/Processor.hpp"
#include <sys/types.h>
#include <sys/sysctl.h>

uint32_t OsGetProcessorCount() {
    uint32_t count = 1;
    uint32_t logicalcpu;
    int32_t length = sizeof(logicalcpu);
    if (sysctlbyname("hw.logicalcpu", &logicalcpu, &length, nullptr, 0) == 0) {
        count = logicalcpu;
    }
    return count;
}

uint32_t OsGetProcessorFeaturesEx(int32_t& vendorID) {
    // https://en.wikipedia.org/wiki/Mac_transition_to_Intel_processors
    vendorID = 1;

    static uint32_t features = 0;

    if (!features) {
        features = 0x80000000;

        int32_t feature;
        int32_t length;

        length = sizeof(feature);
        if (sysctlbyname("hw.optional.mmx", &feature, &length, nullptr, 0) == 0 && feature) {
            features |= 0x2;
        }
        length = sizeof(feature);
        if (sysctlbyname("hw.optional.sse", &feature, &length, nullptr, 0) == 0 && feature) {
            features |= 0x4;
        }
        length = sizeof(feature);
        if (sysctlbyname("hw.optional.sse2", &feature, &length, nullptr, 0) == 0 && feature) {
            features |= 0x10;
        }
        length = sizeof(feature);
        if (sysctlbyname("hw.optional.altivec", &feature, &length, nullptr, 0) == 0 && feature) {
            features |= 0x20;
        }
    }

    return features;
}

void OsSystemEnableCpuLog() {
}

uint64_t OsGetProcessorTicksPerSecond() {
    // TODO
    return 1000ULL;
}

#endif