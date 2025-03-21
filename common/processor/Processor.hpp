#ifndef COMMON_PROCESSOR_PROCESSOR_HPP
#define COMMON_PROCESSOR_PROCESSOR_HPP

#include <cstdint>

uint32_t OsGetProcessorCount();

uint32_t OsGetProcessorFeatures();

uint32_t OsGetProcessorFeaturesEx(int32_t& vendorID);

void OsSystemEnableCpuLog();

uint64_t OsGetProcessorTicksPerSecond();

#endif