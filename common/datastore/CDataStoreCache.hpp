#ifndef COMMON_DATASTORE_C_DATA_STORE_CACHE_HPP
#define COMMON_DATASTORE_C_DATA_STORE_CACHE_HPP

#include "common/datastore/CDataStore.hpp"
#include <cstdlib>
#include <bc/Memory.hpp>

template <size_t size>
class CDataStoreCache : public CDataStore {
    public:
    // Member variables
    uint8_t m_cache[size];

    // Virtual member functions
    virtual void InternalInitialize(uint8_t*& data, uint32_t& base, uint32_t& alloc);
    virtual void InternalDestroy(uint8_t*& data, uint32_t& base, uint32_t& alloc);
    virtual ~CDataStoreCache();

    // Member functions
    CDataStoreCache() {
        this->InternalInitialize(this->m_data, this->m_base, this->m_alloc);
    }
    void Destroy();
};

template <size_t size>
CDataStoreCache<size>::~CDataStoreCache() {
    this->Destroy();
}

template <size_t size>
void CDataStoreCache<size>::Destroy() {
    if (this->m_alloc != -1) {
        this->InternalDestroy(this->m_data, this->m_base, this->m_alloc);
    }
}

template <size_t size>
void CDataStoreCache<size>::InternalInitialize(uint8_t*& data, uint32_t& base, uint32_t& alloc) {
    data = this->m_cache;
    alloc = size;
}

template <size_t size>
void CDataStoreCache<size>::InternalDestroy(uint8_t*& data, uint32_t& base, uint32_t& alloc) {
    if (data && data != this->m_cache) {
        SMemFree(data);
    }

    data = nullptr;
    alloc = 0;
}

#endif
