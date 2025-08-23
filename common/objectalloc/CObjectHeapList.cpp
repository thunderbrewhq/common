#include "common/objectalloc/CObjectHeapList.hpp"
#include <storm/Error.hpp>

CObjectHeapList::CObjectHeapList(const CObjectHeapList& list) {
    if (this == &list) {
        return;
    }

    this->m_heaps.Set(list.m_heaps.Count(), list.m_heaps.Ptr());

    this->m_objSize = list.m_objSize;
    this->m_objsPerBlock = list.m_objsPerBlock;
    this->m_numFullHeaps = list.m_numFullHeaps;
    this->m_hasEmptyHeaps = list.m_hasEmptyHeaps;
    this->uint20 = list.uint20;
    this->m_fullestHeap = list.m_fullestHeap;
    memcpy(this->m_heapName, list.m_heapName, sizeof(this->m_heapName));
    this->uint78 = list.uint78;
    this->uint7C = list.uint7C;
    this->uint80 = list.uint80;
    this->m_freeEmptyHeaps = list.m_freeEmptyHeaps;
}

int32_t CObjectHeapList::New(uint32_t* index, void** obj, bool zero) {
    STORM_ASSERT(index);

    CObjectHeap* heap = nullptr;

    if (this->m_fullestHeap < this->m_heaps.Count()) {
        heap = &this->m_heaps[this->m_fullestHeap];
    }

    if (!heap || heap->m_allocated == this->m_objsPerBlock || !heap->m_obj) {
        if (this->m_heaps.Count() == this->m_numFullHeaps) {
            this->m_fullestHeap = this->m_heaps.Count();

            heap = this->m_heaps.New();

            if (!heap->Allocate(this->m_objSize, this->m_objsPerBlock, this->m_heapName)) {
                return 0;
            }
        } else {
            for (uint32_t i = 0; i < this->m_heaps.Count(); ++i) {
                auto currentHeap = &this->m_heaps[i];
                if (currentHeap->m_allocated != this->m_objsPerBlock) {
                    this->m_fullestHeap = i;
                    heap = currentHeap;
                    if (currentHeap->m_obj) {
                        break;
                    }
                }
            }
        }
    }

    if (!heap) {
        return 0;
    }

    if (!heap->New(this->m_objSize, this->m_objsPerBlock, index, this->m_heapName, obj, zero)) {
        return 0;
    }

    *index += this->m_fullestHeap * this->m_objsPerBlock;

    if (heap->m_allocated == this->m_objsPerBlock) {
        this->m_numFullHeaps++;
    }

    return 1;
}

void* CObjectHeapList::Ptr(uint32_t index) {
    uint32_t heapId = index / this->m_objsPerBlock;
    uint32_t stackId = index % this->m_objsPerBlock;

    STORM_ASSERT(heapId < this->m_heaps.Count());
    return this->m_heaps[heapId].Ptr(stackId, this->m_objSize, this->m_objsPerBlock);
}

void CObjectHeapList::Delete(uint32_t index) {
    uint32_t heapId = index / this->m_objsPerBlock;
    uint32_t stackId = index % this->m_objsPerBlock;

    STORM_ASSERT(heapId < this->m_heaps.Count());
    auto& heap = this->m_heaps[heapId];
    if (heap.m_allocated == this->m_objsPerBlock) {
        this->m_numFullHeaps--;
    }

    heap.Delete(stackId, this->m_objSize, this->m_objsPerBlock);

    if (this->m_freeEmptyHeaps) {
        if (heap.m_obj && !heap.m_allocated) {
            this->m_hasEmptyHeaps = 1;
        }

        if (this->m_hasEmptyHeaps) {
            bool freeHeaps = (((this->m_objsPerBlock - 1) >> 2) & this->uint20) == 0;
            this->uint20++;
            if (freeHeaps) {
                FreeEmptyHeaps();
            }
        }
    }
}

void CObjectHeapList::FreeEmptyHeaps() {
    if (!this->m_heaps.Count()) {
        this->m_hasEmptyHeaps = 0;
        return;
    }

    uint32_t totalAllocated = 0;
    uint32_t totalObjsPerBlock = 0;
    uint32_t totalEmptyHeaps = 0;

    for (uint32_t i = 0; i < this->m_heaps.Count(); ++i) {
        auto& heap = this->m_heaps[i];
        if (heap.m_obj) {
            totalObjsPerBlock += this->m_objsPerBlock;
            if (heap.m_allocated) {
                totalAllocated += heap.m_allocated;
            } else {
                totalEmptyHeaps++;
            }
        }
    }

    if (!totalEmptyHeaps) {
        this->m_hasEmptyHeaps = 0;
        return;
    }

    totalAllocated += ((3 * this->m_objsPerBlock) >> 1);

    uint32_t count = this->m_heaps.Count();

    while (count > 0 && totalAllocated < totalObjsPerBlock && !totalEmptyHeaps) {
        auto& heap = this->m_heaps[count - 1];
        if (heap.m_obj && !heap.m_allocated) {
            heap.Free();
            totalObjsPerBlock -= this->m_objsPerBlock;
            totalEmptyHeaps--;
            this->uint20 = 0;
        }
        count--;
    }

    this->m_hasEmptyHeaps = totalEmptyHeaps != 0;
}

uint32_t CObjectHeapList::BlocksAllocated() {
    uint32_t result = 0;
    for (uint32_t i = 0; i < this->m_heaps.Count(); ++i) {
        result += this->m_heaps[i].m_allocated;
    }

    return result;
}
