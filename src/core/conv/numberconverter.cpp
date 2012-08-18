
#include "numberconverter.h"

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

NumberConverter::NumberConverter():
   mpFormat(NULL)
{
   UErrorCode error = U_ZERO_ERROR;
   mpFormat = NumberFormat::createInstance(Locale::getEnglish(), error);
}

// - Conversions

int NumberConverter::toInt(const String& value)
{
   Formattable result;
   UErrorCode error = U_ZERO_ERROR;
   mpFormat->parse(value.mString, result, error);
   return result.getLong();
}

double NumberConverter::toDouble(const String& value)
{
   bool only = mpFormat->isParseIntegerOnly();

   Formattable format;
   UErrorCode error = U_ZERO_ERROR;
   mpFormat->parse(value.mString, format, error);

   double result = 0;
   switch ( format.getType() )
   {
   case Formattable::kDouble:
      result = format.getDouble();
      break;
   case Formattable::kLong:
      result = format.getLong();
      break;
   }
   return result;
}

String& NumberConverter::format(String& result, int value)
{
   mpFormat->format(value, result.mString);
   return result;
}

String& NumberConverter::format(String& result, double value)
{
   mpFormat->format(value, result.mString);
   return result;
}
