#include "common/DataAllocator.hpp"
#include "test/Test.hpp"

struct TestContainer
{
    uint32_t value[10];
};

class TestClass
{
    public:
    uint32_t* m_value = nullptr;

    TestClass(uint32_t* value) {
        this->m_value = value;
        if (this->m_value) {
            *this->m_value = 1;
        }
    }
    ~TestClass() {
        if (this->m_value) {
            *this->m_value = 2;
        }
    }
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
        allocator.PutData(container, __FILE__, __LINE__);
        allocator.PutData(container2, __FILE__, __LINE__);
        allocator.PutData(container3, __FILE__, __LINE__);
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
        allocator.PutData(container2, __FILE__, __LINE__);
    }
}

TEST_CASE("CDataAllocator Class Handling", "[dataallocator]") {
    SECTION("test class creation/destruction") {
        CDataAllocator allocator(sizeof(TestClass), 2);
        uint32_t value = 0;

        auto testClass = ALLOCATOR_NEW(allocator, TestClass, &value);
        REQUIRE(testClass->m_value == &value);
        REQUIRE(value == 1);

        ALLOCATOR_DEL(allocator, testClass);
        REQUIRE(value == 2);
    }
}
