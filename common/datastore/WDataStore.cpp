#include "common/datastore/WDataStore.hpp"
#include "common/ObjectAlloc.hpp"
#include <bc/Memory.hpp>

static bool s_heapsInitialized = false;
static uint32_t* s_smallHeap = nullptr;
static uint32_t* s_largeHeap = nullptr;
static uint32_t* s_superHeap = nullptr;

void WDataStore::StaticInitialize() {
    if (s_heapsInitialized) {
        return;
    }

    s_smallHeap = NEW(uint32_t);
    *s_smallHeap = ObjectAllocAddHeap(800, 256, "WDataStoreSmallBuffer", true);

    s_largeHeap = NEW(uint32_t);
    *s_largeHeap = ObjectAllocAddHeap(16416, 16, "WDataStoreLargeBuffer", true);

    s_superHeap = NEW(uint32_t);
    *s_superHeap = ObjectAllocAddHeap(262240, 4, "WDataStoreSuperBuffer", true);
}

void WDataStore::StaticDestroy() {
    DEL(s_smallHeap);
    DEL(s_largeHeap);
    DEL(s_superHeap);

    s_heapsInitialized = false;
}

void* WDataStore::AllocBuffer(uint32_t size) {
    uint32_t* heap = nullptr;

    if (size <= 768) {
        heap = s_smallHeap;
    } else if (size <= 16384) {
        heap = s_largeHeap;
    } else if (size <= 262208) {
        heap = s_superHeap;
    }

    if (heap) {
        uint32_t handle = 0;
        uint32_t* result = nullptr;
        if (!ObjectAlloc(*heap, &handle, reinterpret_cast<void**>(&result), false)) {
            return reinterpret_cast<void*>(8 * sizeof(uint32_t));
        } else {
            *result = handle;
            return result + 8;
        }
    } else {
        return ALLOC(size);
    }
}

void WDataStore::FreeBuffer(void* buffer, uint32_t size) {
    uint32_t* heap = nullptr;

    if (size <= 768) {
        heap = s_smallHeap;
    }

    if (size <= 16384) {
        heap = s_largeHeap;
    }

    if (size <= 262208) {
        heap = s_superHeap;
    }

    if (heap) {
        ObjectFree(*heap, *((uint32_t*)buffer - 8));
    } else {
        FREE(buffer);
    }
}

WDataStore::~WDataStore() {

}
