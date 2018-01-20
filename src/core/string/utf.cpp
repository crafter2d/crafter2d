
#include "utf.h"

#include <codecvt>
#include <locale>
#include <string>

/*
std::u32string conv_utf8_to_utf32(const char* const psrc, std::size_t length)
{
   std::wstring_convert<std::codecvt_utf8<__int32>, __int32> utf32conv;
   return (std::u32string)(utf32conv.from_bytes(psrc, &psrc[length]));
}

std::u32string conv_utf8_to_utf32(const std::string& src)
{
   std::wstring_convert<std::codecvt_utf8<__int32>, __int32> utf32conv;
   return utf32conv.from_bytes(src);
}

std::string conv_utf16_to_utf8(const std::u16string& src)
{
   std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
   return conv.to_bytes(src);
}

std::string conv_utf32_to_utf8(const std::u32string& src)
{
   std::wstring_convert<std::codecvt_utf8<__int32>, __int32> conv;
   return conv.to_bytes(src);
}
*/

using Converter = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>;

Converter& getConverter()
{
   static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
   return converter;
}

std::string conv_wchar_to_utf8(const std::wstring& src)
{
   return getConverter().to_bytes(src);
}

std::wstring conv_utf8_to_wchar(const char* const psrc, std::size_t length)
{
   if ( length == 0 )
      return getConverter().from_bytes(psrc);
   else
      return getConverter().from_bytes(psrc, &psrc[length]);
}

std::wstring conv_utf8_to_wchar(const std::string& src)
{
   return getConverter().from_bytes(src);
}
