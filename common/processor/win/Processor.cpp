#if defined(WHOA_SYSTEM_WIN)

#include "common/processor/Processor.hpp"
#include "common/time/Time.hpp"
#include <cstring>
#include <storm/Log.hpp>
#include <bc/os/File.hpp>
#include <windows.h>

uint32_t OsGetProcessorCount() {
    SYSTEM_INFO systeminfo;
    memset(&systeminfo, 0, sizeof(systeminfo));
    GetSystemInfo(&systeminfo);
    return systeminfo.dwNumberOfProcessors ? systeminfo.dwNumberOfProcessors : 1;
}

// NOTE: this is is an x86-only structure
struct ProcessorFeatures {
    // vendor ID string
    uint32_t std_0b; // 00
    uint32_t std_0d; // 04
    uint32_t std_0c; // 08
    // standard
    uint32_t std_0a; // 0C

    uint32_t std_1b; // 10
    uint32_t std_1d; // 14
    uint32_t std_4a; // 18
    // extended
    uint32_t ext_0a; // 1C

    uint32_t ext_1c; // 20
    uint32_t ext_1d; // 24
    uint32_t ext_8c; // 28

    // processor brand string
    uint32_t ext_2a; // 2C
    uint32_t ext_2b; // 30
    uint32_t ext_2c; // 34
    uint32_t ext_2d; // 38

    uint32_t ext_3a; // 3C
    uint32_t ext_3b; // 40
    uint32_t ext_3c; // 44
    uint32_t ext_3d; // 48

    uint32_t ext_4a; // 4c
    uint32_t ext_4b; // 50
    uint32_t ext_4c; // 54
    uint32_t ext_4d; // 58
};

// MinGW implementation

#if defined(__MINGW32__) && (defined(__x86_64__) || defined(__i386__))

#include <cpuid.h>

#define COMMON_GET_PROCESSOR_FEATURES_IMPL 1

int32_t IOsGetProcessorFeatures(ProcessorFeatures& features) {
    memset(&features, 0, sizeof(ProcessorFeatures));

    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;

    int32_t result = 0;

    if (__get_cpuid(0, &eax, &ebx, &ecx, &edx)) {
        result = 1;
        features.std_0a = eax;
        features.std_0b = ebx;
        features.std_0d = edx;
        features.std_0c = ecx;

        if (features.std_0a >= 4) {
            __get_cpuid(4, &eax, &ebx, &ecx, &edx);
            features.std_4a = eax;
        }

        __get_cpuid(1, &eax, &ebx, &ecx, &edx);
        features.std_1d = edx;
        features.std_1b = ebx;

        __get_cpuid(0x80000000, &eax, &ebx, &ecx, &edx);
        if (eax > 0x80000000) {
            result = 2;
            features.ext_0a = eax;
            if (features.ext_0a >= 0x80000008) {
                __get_cpuid(0x80000008, &eax, &ebx, &ecx, &edx);
                features.ext_8c = ecx;
            }
            if (features.ext_0a >= 0x80000002) {
                __get_cpuid(0x80000002, &eax, &ebx, &ecx, &edx);
                features.ext_2a = eax;
                features.ext_2b = ebx;
                features.ext_2c = ecx;
                features.ext_2d = edx;
            }
            if (features.ext_0a >= 0x80000003) {
                __get_cpuid(0x80000003, &eax, &ebx, &ecx, &edx);
                features.ext_3a = eax;
                features.ext_3b = ebx;
                features.ext_3c = ecx;
                features.ext_3d = edx;
            }
            if (features.ext_0a >= 0x80000004) {
                __get_cpuid(0x80000004, &eax, &ebx, &ecx, &edx);
                features.ext_4a = eax;
                features.ext_4b = ebx;
                features.ext_4c = ecx;
                features.ext_4d = edx;
            }
            __get_cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
            features.ext_1d = edx;
            features.ext_1c = ecx;
        }
    }

    return result;
}

#endif

#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))

// MSVC implementation

#include <intrin.h>

int32_t IOsGetProcessorFeatures(ProcessorFeatures& features) {
    memset(&features, 0, sizeof(ProcessorFeatures));

    int cpuinfo[4];

    int32_t result = 0;

    __cpuid(0, cpuinfo);
    if (cpuinfo[0]) {
        result = 1;
        features.std_0a = cpuinfo[0];
        features.std_0b = cpuinfo[1];
        features.std_0d = cpuinfo[3];
        features.std_0c = cpuinfo[2];

        if (features.std_0a >= 4) {
            __cpuid(4, cpuinfo);
            features.std_4a = cpuinfo[0];
        }

        __cpuid(1, cpuinfo);
        features.std_1d = cpuinfo[3];
        features.std_1b = cpuinfo[0];

        __cpuid(0x80000000, cpuinfo);
        if (cpuinfo[0] > 0x80000000) {
            result = 2;
            features.ext_0a = cpuinfo[0];
            if (features.ext_0a >= 0x80000008) {
                __cpuid(0x80000008, cpuinfo);
                features.ext_8c = cpuinfo[2];
            }
            if (features.ext_0a >= 0x80000002) {
                __cpuid(0x80000002, cpuinfo);
                features.ext_2a = cpuinfo[0];
                features.ext_2b = cpuinfo[1];
                features.ext_2c = cpuinfo[2];
                features.ext_2d = cpuinfo[3];
            }
            if (features.ext_0a >= 0x80000003) {
                __cpuid(0x80000003, cpuinfo);
                features.ext_3a = cpuinfo[0];
                features.ext_3b = cpuinfo[1];
                features.ext_3c = cpuinfo[2];
                features.ext_3d = cpuinfo[3];
            }
            if (features.ext_0a >= 0x80000004) {
                __cpuid(0x80000004, cpuinfo);
                features.ext_4a = cpuinfo[0];
                features.ext_4b = cpuinfo[1];
                features.ext_4c = cpuinfo[2];
                features.ext_4d = cpuinfo[3];
            }
            __cpuid(0x80000001, cpuinfo);
            features.ext_1d = cpuinfo[3];
            features.ext_1c = cpuinfo[2];
        }
    }

    return result;
}

#define COMMON_GET_PROCESSOR_FEATURES_IMPL 1

#endif

#if !defined(COMMON_GET_PROCESSOR_FEATURES_IMPL)

// placeholder implementation

int32_t IOsGetProcessorFeatures(ProcessorFeatures& features) {
    memset(&features, 0, sizeof(ProcessorFeatures));

    return 0;
}

#endif

static uint64_t s_cpuTicksPerSecond;

static int32_t s_cpuLogEnabled;

static int32_t s_haveProcessorFeatures;
static uint32_t s_processorFeatures;
static uint32_t s_processorCount;
static uint32_t s_processorCores;
static uint32_t s_processorVendor;
static uint32_t s_processorSockets;

static const char s_vendorIDs[] = {
    'G', 'e', 'n', 'u', 'i', 'n', 'e', 'I', 'n', 't', 'e', 'l',
    'A', 'u', 't', 'h', 'e', 'n', 't', 'i', 'c', 'A', 'M', 'D',
    'C', 'y', 'r', 'i', 'x', 'I', 'n', 's', 't', 'e', 'a', 'd',
    'C', 'e', 'n', 't', 'a', 'u', 'r', 'H', 'a', 'l', 'l', 's'
};

void OsSystemEnableCpuLog() {
    s_cpuLogEnabled = 1;
}

void IOsSystemCpuLog(ProcessorFeatures& features) {
    if (s_cpuLogEnabled) {
        OsCreateDirectory("Logs", 0);
        HSLOG log;
        SLogCreate("Logs\\cpu.log", 0, &log);
        if (s_processorVendor == 4) {
            SLogWrite(log, "UNABLE TO IDENTIFY CPU");
            SLogClose(log);
            return;
        }
        SLogWrite(log, "vendor: %d", s_processorVendor);
        SLogWrite(log, "features: %08X", s_processorFeatures & 0x7fffffff);
        SLogWrite(log, "sockets: %d", s_processorSockets);
        SLogWrite(log, "cores: %d", s_processorCores);
        SLogWrite(log, "processors: %d", s_processorCount);

        char vendorID[13];
        strncpy(vendorID, reinterpret_cast<char*>(&features), 12);
        vendorID[12] = '\0';
        SLogWrite(log, "vendor id string= %s", vendorID);

        SLogWrite(log, "standard (%d): 1b=%08X 1d=%08x 4a=%08X", features.std_0a, features.std_1b, features.std_1d, features.std_4a);
        SLogWrite(log, "extended (%d): 1c=%08X 1d=%08x 8c=%08X", features.ext_0a & 0x7fffffff, features.ext_1c, features.ext_1d, features.ext_8c);

        auto brand = reinterpret_cast<const char*>(&features.ext_2a);
        while (*brand && *brand == ' ') {
            brand++;
        }
        SLogWrite(log, "processor brand string= %s", brand);
        SLogClose(log);
    }
}

int32_t IOsParseProcessorFrequency(const char* str) {
    if (!str) {
        return 0;
    }

    auto length = strlen(str);
    // z
    auto z = &str[length - 1];
    if (z <= str) {
        return 0;
    }
    if (*z != 'z') {
        return 0;
    }
    // Hz
    auto H = &str[length - 2];
    if (H <= str) {
        return 0;
    }
    if (*H != 'H') {
        return 0;
    }
    // GHz, MHz, THz (!)
    auto s = &str[length - 3];
    if (s <= str) {
        return 0;
    }
    double scale;
    if (*s == 'G') {
        scale = 1000000000.0;
    } else if (*s == 'M') {
        scale = 1000000.0;
    } else if (*s == 'T') {
        scale = 1000000000000.0;
    } else {
        return 0;
    }

    // quantity (floating-point)
    auto q = &str[length - 4];
    char quantitystr[48];
    memset(quantitystr, 0, sizeof(quantitystr));
    quantitystr[47] = '\0';
    for (auto p = quantitystr + 46; q > str; p--) {
        if (!*q || (*q < '0' || *q > '9') && *q != '.') {
            break;
        }
        *p = *q;
        q--;
    }
    auto quantity = atof(quantitystr);
    if (quantity == 0.0) {
        return 0;
    }
    s_cpuTicksPerSecond = static_cast<uint64_t>(quantity * scale);
    return 1;
}

int32_t IOsGetPowerProfFrequency() {
    auto library = LoadLibrary(TEXT("powrprof.dll"));
    if (!library) {
        return 0;
    }

    int32_t result = 0;

    typedef NTSTATUS (*LPCALLNTPOWERINFORMATIONFUNC)(POWER_INFORMATION_LEVEL, PVOID, ULONG, PVOID, ULONG);
    auto CallNtPowerInformation = reinterpret_cast<LPCALLNTPOWERINFORMATIONFUNC>(GetProcAddress(library, "CallNtPowerInformation"));
    if (CallNtPowerInformation) {
        struct PROCESSOR_POWER_INFORMATION {
            ULONG Number;
            ULONG MaxMhz;
            ULONG CurrentMhz;
            ULONG MhzLimit;
            ULONG MaxIdleState;
            ULONG CurrentIdleState;
        };
        PROCESSOR_POWER_INFORMATION info[4];
        if (!CallNtPowerInformation(ProcessorInformation, nullptr, 0, &info, sizeof(info))) {
            s_cpuTicksPerSecond = 1000000LL * static_cast<int64_t>(info[0].MaxMhz);
            result = 1;
        }
    }

    FreeLibrary(library);
    return result;
}

uint32_t OsGetProcessorFeaturesEx(int32_t& vendorID) {
    if (s_haveProcessorFeatures) {
        vendorID = s_processorVendor;
        return s_processorFeatures;
    }

    s_haveProcessorFeatures = 1;

    s_processorCores = 1;
    s_processorSockets = 1;

    s_processorFeatures = 0x80000000;

    ProcessorFeatures features;
    if (!IOsGetProcessorFeatures(features)) {
        s_processorVendor = 4;
    } else {
        if ((features.std_1d & 0x800000) != 0) {
            s_processorFeatures |= 0x2;
        }
        if ((features.std_1d & 0x2000000) != 0) {
            s_processorFeatures |= 0x4;
        }
        if ((features.ext_1d & 0x80000000) != 0) {
            s_processorFeatures |= 0x8;
        }
        if ((features.std_1d & 0x4000000) != 0) {
            s_processorFeatures |= 0x10;
        }

        auto vendorstring = reinterpret_cast<char*>(&features);

        // Intel
        if (!strncmp(vendorstring, s_vendorIDs, 12)) {
            s_processorVendor = 1;
            auto cores = (features.std_4a >> 0x1A);
            if (features.std_0a >= 4 && cores) {
                s_processorFeatures |= 0x40;
                s_processorCores = cores + 1;
            }
            // AMD
        } else if (!strncmp(vendorstring, s_vendorIDs + 12, 12)) {
            s_processorVendor = 2;
            auto cores = static_cast<uint8_t>(features.ext_8c);
            if (cores) {
                s_processorFeatures |= 0x40;
                s_processorCores = cores + 1;
            }
            // Cyrix
        } else if (!strncmp(vendorstring, s_vendorIDs + 24, 12)) {
            // ???
            // Centaur
        } else if (!strncmp(vendorstring, s_vendorIDs + 36, 12)) {
            // ???
        }

        IOsSystemCpuLog(features);
    }

    if (!IOsParseProcessorFrequency(reinterpret_cast<char*>(&features) + 44)) {
        static int32_t s_checkedPowerProfInfo = 0;
        static int32_t s_gotPowerProfFrequency = 0;

        if (!s_checkedPowerProfInfo) {
            s_gotPowerProfFrequency = IOsGetPowerProfFrequency();
            s_checkedPowerProfInfo = 1;
        }

        if (!s_gotPowerProfFrequency) {
            s_cpuTicksPerSecond = OsGetAsyncClocksPerSecond();
        }
    }

    vendorID = s_processorVendor;
    return s_processorFeatures;
}

uint64_t OsGetProcessorTicksPerSecond() {
    if (!s_cpuTicksPerSecond) {
        int32_t vendorID;
        OsGetProcessorFeaturesEx(vendorID);
    }

    return s_cpuTicksPerSecond;
}

#endif