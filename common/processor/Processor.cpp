#include "common/processor/Processor.hpp"

uint32_t OsGetProcessorFeatures() {
    int32_t vendorID;
    return OsGetProcessorFeaturesEx(vendorID);
}