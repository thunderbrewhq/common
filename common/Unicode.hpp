#ifndef COMMON_UNICODE_HPP
#define COMMON_UNICODE_HPP

#include <cstdint>

uint32_t sgetu8(const uint8_t* strptr, int32_t* chars);

void sputu8(uint32_t c, char* strptr);

int32_t ConvertUTF16to8(uint8_t* dst, uint32_t dstmaxchars, const uint16_t* src, uint32_t srcmaxchars, uint32_t* dstchars, uint32_t* srcchars);

int32_t ConvertUTF8to16(uint16_t* dst, uint32_t dstmaxchars, const uint8_t* src, uint32_t srcmaxchars, uint32_t* dstchars, uint32_t* srcchars);

#endif
