#ifndef COMMON_DATASTORE_W_DATA_STORE_HPP
#define COMMON_DATASTORE_W_DATA_STORE_HPP

#include "common/datastore/CDataStore.hpp"

class WDataStore : public CDataStore {
    public:
    // Member variables
    void* m_bufferObj = nullptr;

    // Static functions
    static void StaticInitialize();
    static void StaticDestroy();
    static void* AllocBuffer(uint32_t size);
    static void FreeBuffer(void* buffer, uint32_t size);

    // Virtual member functions
    virtual ~WDataStore();

    virtual void InternalInitialize(uint8_t*& data, uint32_t& base, uint32_t& alloc);
    virtual void InternalDestroy(uint8_t*& data, uint32_t& base, uint32_t& alloc);
    virtual int32_t InternalFetchWrite(uint32_t pos, uint32_t bytes, uint8_t*& data, uint32_t& base, uint32_t& alloc, const char* fileName, int32_t lineNumber);
};

#endif
