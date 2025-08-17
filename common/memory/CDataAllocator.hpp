#ifndef COMMON_MEMORY_C_DATA_ALLOCATOR_HPP
#define COMMON_MEMORY_C_DATA_ALLOCATOR_HPP

#include <cstdint>
#include <cstdlib>
#include <new>
#include <type_traits>

#define ALLOCATOR_GET(allocator) allocator.GetData(0, __FILE__, __LINE__)
#define ALLOCATOR_GET_ZERO(allocator) allocator.GetData(1, __FILE__, __LINE__)
#define ALLOCATOR_NEW(allocator, T, ...) (new (allocator.GetData(0, __FILE__, __LINE__)) T(__VA_ARGS__))
#define ALLOCATOR_NEW_ZERO(allocator, T, ...) (new (allocator.GetData(1, __FILE__, __LINE__)) T(__VA_ARGS__))

#define ALLOCATOR_PUT(allocator, ptr) allocator.PutData(ptr, __FILE__, __LINE__)
#define ALLOCATOR_PUT(allocator, ptr)                                                                               \
    do {                                                                                       \
        if (ptr) {                                                                             \
            using __data_object = std::remove_pointer<std::decay<decltype(ptr)>::type>::type;  \
            (ptr)->~__data_object();                                                           \
            allocator.PutData(ptr, __FILE__, __LINE__);                                        \
        }                                                                                      \
    } while (0)


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

    inline uint32_t BytesPerData() const { return this->m_bytesPerData; }
    inline uint32_t DataPerBlock() const { return this->m_dataPerBlock; }
    inline uint32_t DataUsed() const { return this->m_dataUsed; }

    private:
    // Member variables
    uint32_t m_bytesPerData = sizeof(Data);
    uint32_t m_dataPerBlock = 1;
    uint32_t m_dataUsed = 0;
    Block* m_blockList = nullptr;
    Data* m_dataList = nullptr;
};

#endif
