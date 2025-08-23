#ifndef COMMON_OBJECTALLOC_C_OBJECT_HEAP_HPP
#define COMMON_OBJECTALLOC_C_OBJECT_HEAP_HPP

#include <cstdint>

class CObjectHeap {
    public:
    // Member variables
    void* m_obj = nullptr;
    uint32_t* m_indexStack = nullptr;
    uint32_t m_allocated = 0;

    // Member functions
    CObjectHeap() = default;
    ~CObjectHeap();

    int32_t Allocate(uint32_t objSize, uint32_t heapObjects, const char* heapName);
    int32_t New(uint32_t objSize, uint32_t heapObjects, uint32_t* index, const char* heapName, void** obj, bool zero);
    void* Ptr(uint32_t index, uint32_t objSize, uint32_t heapObjects);
    void Delete(uint32_t index, uint32_t objSize, uint32_t heapObjects);
    void Free();
};

#endif
