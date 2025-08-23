#ifndef COMMON_OBJECT_ALLOC_HPP
#define COMMON_OBJECT_ALLOC_HPP

#include <cstdint>

int32_t ObjectAlloc(uint32_t heapId, uint32_t* memHandle, void** objectPtr, bool zero);

uint32_t ObjectAllocAddHeap(uint32_t objectSize, uint32_t objsPerBlock, const char* name, bool freeEmptyHeaps);

uint32_t ObjectAllocUsage(uint32_t heapId);

void* ObjectPtr(uint32_t heapId, uint32_t memHandle);

void ObjectFree(uint32_t heapId, uint32_t memHandle);

void ObjectAllocDestroy();

#endif
