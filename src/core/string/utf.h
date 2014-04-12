
#ifndef UTF_H
#define UTF_H

#include <cstdint>

uint32_t conv_utf8_to_utf16(wchar_t* pdest, uint32_t size, const char* psrc, uint32_t length);
char* conv_utf16_to_utf8(const wchar_t* psrc, uint32_t length);

#endif // UTF_H
