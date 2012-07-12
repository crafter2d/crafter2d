
#include "numberconverter.h"

#include "core/defines.h"
#include "core/string/string.h"

// static
NumberConverter& NumberConverter::getInstance()
{
   static NumberConverter* pconverter = new NumberConverter();
   return *pconverter;
}

// - Interface

NumberConverter::NumberConverter():
   mpFormat(NULL)
{
   UErrorCode error;
   mpFormat = NumberFormat::createInstance(error);
}

// - Conversions

int NumberConverter::toInt(const String& value)
{
   UErrorCode error;
   Formattable result;
   mpFormat->parse(value.mString, result, error);
   ASSERT(result.getType() == Formattable::kLong);
   return result.getLong(NULL);
}

double NumberConverter::toDouble(const String& value)
{
   UErrorCode error;
   Formattable result;
   mpFormat->parse(value.mString, result, error);
   ASSERT(result.getType() == Formattable::kLong);
   return result.getDouble(error);
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
