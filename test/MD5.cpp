#include "common/MD5.hpp"
#include "test/Test.hpp"

#include <cstdio>
#include <cstring>

static bool test_hash(const char* data, const char* expected, int length = -1) {
    const size_t digestLength = 16;

    uint8_t digest[digestLength] = {};
    char hex[(2 * digestLength) + 1];

    if (length < 0) {
        length = strlen(data);
    }

    /* calculate hash */
    MD5_CTX context;
    MD5Init(&context);
    MD5Update(&context, data, static_cast<size_t>(length));
    MD5Final(digest, &context);

    /* copy hash to string */
    for (size_t i = 0; i < digestLength; i++) {
        sprintf(&(hex[i * 2]), "%02x", digest[i]);
    }
    hex[digestLength * 2] = '\0';

    /* compare with result string */
    return strcmp(hex, expected) == 0;
}

TEST_CASE("MD5", "[md5]") {
    SECTION("zero data") {
        REQUIRE(test_hash("", "d41d8cd98f00b204e9800998ecf8427e"));
    }
    SECTION("test 1") {
        REQUIRE(test_hash("a", "0cc175b9c0f1b6a831c399e269772661"));
    }
    SECTION("test 2") {
        REQUIRE(test_hash("abc", "900150983cd24fb0d6963f7d28e17f72"));
    }
    SECTION("test 3") {
        REQUIRE(test_hash(
            "12345678901234567890123456789012345678901234567890123456789012345678901234567890",
            "57edf4a22be3c955ac49da2e2107b67a"));
    }
    SECTION("binary array test") {
        char data[8192];
        for (size_t i = 0; i < sizeof(data); ++i) {
            data[i] = static_cast<char>(i % 256);
        }
        test_hash(data, "6556112372898c69e1de0bf689d8db26", sizeof(data));
    }
}
