#if defined(WHOA_SYSTEM_LINUX)

#include "common/processor/Processor.hpp"
#include <unistd.h>

uint32_t OsGetProcessorCount() {
    return sysconf(_SC_NPROCESSORS_ONLN);
}

uint32_t OsGetProcessorFeaturesEx(int32_t& vendorID) {


    // TODO
    vendorID = 1;
    return 0x80000000;
}

void OsSystemEnableCpuLog() {
    // TODO
}

uint64_t OsGetProcessorTicksPerSecond() {
    // TODO
    return 1000ULL;
}

#endif