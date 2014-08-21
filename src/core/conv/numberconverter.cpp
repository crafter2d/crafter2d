
#include "numberconverter.h"

#include <string.h>

#include "core/defines.h"
#include "core/string/string.h"

// static
NumberConverter& NumberConverter::getInstance()
{
   static NumberConverter* pconverter;
   if ( pconverter == NULL )
   {
      pconverter = new NumberConverter();
   }
   return *pconverter;
}

// - Interface

NumberConverter::NumberConverter()
{
}

// - Conversions

int NumberConverter::toInt(UChar value)
{
   return wcstol(&value, NULL, 10);
}

int NumberConverter::toInt(const String& value)
{
   return wcstol(value.mpString, NULL, 10);
}

float NumberConverter::toFloat(const String& value)
{
   return wcstof(value.mpString, NULL);
}

double NumberConverter::toDouble(const String& value)
{
   return wcstod(value.mpString, NULL);
}

String& NumberConverter::format(String& result, int value)
{
   wchar_t buff[256];
   memset(buff, 0, sizeof(wchar_t) * 256);
   int len = swprintf(buff, 10, L"%d", value);
   result.setTo(buff, len);
   return result;
}

String& NumberConverter::format(String& result, double value)
{
   wchar_t buff[256];
   memset(buff, 0, sizeof(wchar_t) * 256);
   int len = swprintf(buff, 10, L"%Lf", value);
   result.setTo(buff, len);
   return result;
}
