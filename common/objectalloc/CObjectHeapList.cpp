#include "common/objectalloc/CObjectHeapList.hpp"
#include <storm/Error.hpp>

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

uint32_t CObjectHeapList::BlocksAllocated() {
    uint32_t result = 0;
    for (uint32_t i = 0; i < this->m_heaps.Count(); ++i) {
        result += this->m_heaps[i].m_allocated;
    }

    return result;
}
