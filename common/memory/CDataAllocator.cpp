#include "common/memory/CDataAllocator.hpp"

#include <algorithm>

#include <storm/Error.hpp>
#include <bc/Memory.hpp>

CDataAllocator::CDataAllocator(uint32_t bytesPerData, uint32_t dataPerBlock) {
    this->m_bytesPerData = std::max(bytesPerData, uint32_t(sizeof(Data)));
    this->m_dataPerBlock = std::max(dataPerBlock, uint32_t(1));
}

CDataAllocator::~CDataAllocator() {
    if (this->m_blockList) {
        Clear(__FILE__, __LINE__);
    }
}

void CDataAllocator::Clear(const char* fileName, int32_t lineNumber) {
#if defined(WHOA_BUILD_ASSERTIONS)
    if (this->m_dataUsed) {
        if (!fileName) {
            fileName == __FILE__;
            lineNumber = __LINE__;
        }

        SErrDisplayErrorFmt(
            STORM_ERROR(0x7E),
            fileName,
            lineNumber,
            1,
            1,
            "CDataAllocator@0x%08x: leaked %u: %s(%d) [%u][%u]",
            this,
            this->m_dataUsed,
            fileName,
            lineNumber,
            this->m_dataPerBlock,
            this->m_bytesPerData);
    }
#endif

    while (this->m_blockList) {
        auto block = this->m_blockList;
        this->m_blockList = block->m_next;
        SMemFree(block);
    }

    this->m_dataUsed = 0;
    this->m_dataList = 0;
}

CDataAllocator::Data* CDataAllocator::GetData(int32_t zero, const char* fileName, int32_t lineNumber) {
    if (this->m_dataPerBlock == 1) {
        fileName = nullptr;
        lineNumber = 0;
    }

    if (!this->m_dataList) {
        if (!fileName) {
            fileName = __FILE__;
            lineNumber = __LINE__;
        }

        auto memory = static_cast<char*>(SMemAlloc(
            this->m_dataPerBlock * this->m_bytesPerData + sizeof(Block),
            fileName,
            lineNumber,
            0));
        Block* block = reinterpret_cast<Block*>(memory);
        Data* data = reinterpret_cast<Data*>(memory + sizeof(Block));

        this->m_dataList = data;
        for (uint32_t i = 0; i < this->m_dataPerBlock - 1; ++i) {
            auto next = reinterpret_cast<char*>(data) + this->m_bytesPerData;
            data->m_next = reinterpret_cast<Data*>(next);
            data = data->m_next;
        }
        data->m_next = nullptr;
        block->m_next = this->m_blockList;
        this->m_blockList = block;
    }

    auto data = this->m_dataList;
    this->m_dataList = data->m_next;
    if (zero) {
        memset(data, 0, this->m_bytesPerData);
    }
    this->m_dataUsed++;
    return data;
}

void CDataAllocator::PutData(void* data, const char* fileName, int32_t lineNumber) {
    STORM_ASSERT(this->m_dataUsed > 0);
    auto d = reinterpret_cast<Data*>(data);
    d->m_next = this->m_dataList;
    this->m_dataUsed--;
    this->m_dataList = d;
}
