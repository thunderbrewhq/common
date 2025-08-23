#include "common/objectalloc/CObjectHeap.hpp"
#include <cstring>
#include <cctype>
#include <bc/Memory.hpp>
#include <storm/Error.hpp>

CObjectHeap::~CObjectHeap() {
    this->Free();
}

int32_t CObjectHeap::Allocate(uint32_t objSize, uint32_t heapObjects, const char* heapName) {
    STORM_ASSERT(this->m_obj == nullptr);

    this->m_obj = SMemAlloc(heapObjects * (objSize + sizeof(uint32_t)), heapName, 0, 0x0);

    void* indexStack = static_cast<char*>(this->m_obj) + heapObjects * objSize;
    this->m_indexStack = reinterpret_cast<uint32_t*>(indexStack);

    for (int32_t i = 0; i < heapObjects; i++) {
        this->m_indexStack[i] = i;
    }

    this->m_allocated = 0;

    return this->m_obj != nullptr;
}

int32_t CObjectHeap::New(uint32_t objSize, uint32_t heapObjects, uint32_t* index, const char* heapName, void** obj, bool zero) {
    STORM_ASSERT(index);

    if (this->m_allocated >= heapObjects) {
        return 0;
    }

    if (!this->m_obj) {
        this->Allocate(objSize, heapObjects, heapName);
    }

    STORM_ASSERT(this->m_obj);

    *index = this->m_indexStack[this->m_allocated++];
    void* objPtr = static_cast<char*>(this->m_obj) + objSize * *index;

    if (zero && objPtr) {
        memset(objPtr, 0, objSize);
    }

    if (obj) {
        *obj = objPtr;
    }

    return 1;
}

void* CObjectHeap::Ptr(uint32_t index, uint32_t objSize, uint32_t heapObjects) {
    STORM_ASSERT(this->m_obj);
    if (index >= heapObjects) {
        const char* format = "\"%s\", %s = %ld (0x%08X)";
        const uint8_t bytes[4] = {
            (index >> 24) & 0xFF,
            (index >> 16) & 0xFF,
            (index >> 8) & 0xFF,
            index & 0xFF
        };

        if (isprint(bytes[0])
            && isprint(bytes[1])
            && isprint(bytes[2])
            && isprint(bytes[3])) {
            format = "\"%s\", %s = %ld (0x%08X, '%c%c%c%c')";
        }

        SErrDisplayErrorFmt(
            STORM_ERROR(0),
            __FILE__,
            __LINE__,
            0,
            1,
            format,
            "(index < heapObjects)",
            "index",
            index,
            index,
            bytes[0],
            bytes[1],
            bytes[2],
            bytes[3]);
    }

    return static_cast<char*>(this->m_obj) + objSize * index;
}

void CObjectHeap::Delete(uint32_t index, uint32_t objSize, uint32_t heapObjects) {
    STORM_ASSERT(this->m_obj);
    if (index >= heapObjects) {
        const char* format = "\"%s\", %s = %ld (0x%08X)";
        const uint8_t bytes[4] = {
            (index >> 24) & 0xFF,
            (index >> 16) & 0xFF,
            (index >> 8) & 0xFF,
            index & 0xFF
        };

        if (isprint(bytes[0]) && isprint(bytes[1]) && isprint(bytes[2]) && isprint(bytes[3])) {
            format = "\"%s\", %s = %ld (0x%08X, '%c%c%c%c')";
        }

        SErrDisplayErrorFmt(
            STORM_ERROR(0),
            __FILE__,
            __LINE__,
            0,
            1,
            format,
            "(index < heapObjects)",
            "index",
            index,
            index,
            bytes[0],
            bytes[1],
            bytes[2],
            bytes[3]);
    }
    STORM_ASSERT(this->m_allocated);

    this->m_indexStack[--this->m_allocated] = index;
}

void CObjectHeap::Free() {
    if (this->m_obj) {
        SMemFree(this->m_obj, __FILE__, __LINE__, 0);
    }
    this->m_obj = nullptr;
    this->m_indexStack = nullptr;
    this->m_allocated = 0;
}
