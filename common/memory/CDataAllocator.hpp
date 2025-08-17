#ifndef COMMON_MEMORY_C_DATA_ALLOCATOR_HPP
#define COMMON_MEMORY_C_DATA_ALLOCATOR_HPP

#include <cstdint>

class CDataAllocator {
    public:
    // Sub-types
    struct Block {
        Block* m_next;
    };

    struct Data {
        Data* m_next;
    };

    // Member functions
    CDataAllocator(uint32_t bytesPerData, uint32_t dataPerBlock);
    ~CDataAllocator();
    void Clear(const char* fileName, int32_t lineNumber);
    Data* GetData(int32_t zero, const char* fileName, int32_t lineNumber);
    void PutData(void* data, const char* fileName, int32_t lineNumber);

    // Member variables
    uint32_t m_bytesPerData = sizeof(Data);
    uint32_t m_dataPerBlock = 1;
    uint32_t m_dataUsed = 0;
    Block* m_blockList = nullptr;
    Data* m_dataList = nullptr;
};

#endif
