#include "common/DataAllocator.hpp"
#include "test/Test.hpp"

struct TestContainer
{
    uint32_t value[10];
};

TEST_CASE("CDataAllocator::CDataAllocator", "[dataallocator]") {
    SECTION("constructs new data allocator") {
        CDataAllocator allocator(sizeof(TestContainer), 2);
        SUCCEED();
    }
}

TEST_CASE("CDataAllocator::GetData", "[dataallocator]") {
    SECTION("get allocated objects") {
        CDataAllocator allocator(sizeof(TestContainer), 2);
        auto container = (TestContainer*)allocator.GetData(0, __FILE__, __LINE__);
        auto container2 = (TestContainer*)allocator.GetData(0, __FILE__, __LINE__);
        auto container3 = (TestContainer*)allocator.GetData(0, __FILE__, __LINE__);
        REQUIRE(container);
        REQUIRE(container2);
        REQUIRE(container3);
        REQUIRE(allocator.DataUsed() == 3);
    }
}

TEST_CASE("CDataAllocator::PutData", "[dataallocator]") {
    SECTION("return allocated objects") {
        CDataAllocator allocator(sizeof(TestContainer), 2);
        auto container = (TestContainer*)allocator.GetData(0, __FILE__, __LINE__);
        auto container2 = (TestContainer*)allocator.GetData(0, __FILE__, __LINE__);
        auto container3 = (TestContainer*)allocator.GetData(0, __FILE__, __LINE__);
        allocator.PutData(container, __FILE__, __LINE__);
        allocator.PutData(container3, __FILE__, __LINE__);
        REQUIRE(allocator.DataUsed() == 1);
    }
}
