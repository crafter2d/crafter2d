
#ifndef UTF_H
#define UTF_H

#include <string>

/*
std::u32string conv_utf8_to_utf32(const char* const psrc, std::size_t length);
std::u32string conv_utf8_to_utf32(const std::string& src);
std::string conv_utf32_to_utf8(const std::u32string& src);

std::string conv_utf16_to_utf8(const std::u16string& src);
*/

std::wstring conv_utf8_to_wchar(const char* const psrc, std::size_t length);
std::wstring conv_utf8_to_wchar(const std::string& src);
std::string conv_wchar_to_utf8(const std::wstring& src);

#endif // UTF_H
