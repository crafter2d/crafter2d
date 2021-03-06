
#include "numberconverter.h"

#include <locale>
#include <codecvt>
#include <string>

#include "core/defines.h"
#include "core/string/string.h"

// static
NumberConverter& NumberConverter::getInstance()
{
   static NumberConverter* pconverter;
   if ( pconverter == nullptr )
   {
      pconverter = new NumberConverter();
   }
   return *pconverter;
}

// - Conversions

int NumberConverter::toInt(UChar value)
{
   String str;
   str += value;
   return toInt(str);
}

int NumberConverter::toInt(const String& value)
{
   std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
   return std::stoi(convert.to_bytes(value.mData.c_str()), nullptr, 10);
}

float NumberConverter::toFloat(const String& value)
{
   std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
   return std::stof(convert.to_bytes(value.mData.c_str()), nullptr);
}

double NumberConverter::toDouble(const String& value)
{
   std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
   return std::stod(convert.to_bytes(value.mData.c_str()), nullptr);
}

String& NumberConverter::format(String& result, int value)
{
   auto s = std::to_string(value);
   result.setToUtf8(s);
   return result;
}

String& NumberConverter::format(String& result, unsigned int value)
{
   auto s = std::to_string(value);
   result.setToUtf8(s);
   return result;
}

String& NumberConverter::format(String& result, double value)
{
   auto s = std::to_string(value);
   result.setToUtf8(s);
   return result;
}
