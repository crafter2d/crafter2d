
#ifndef NUMBER_CONVERTER_H
#define NUMBER_CONVERTER_H

#include "core/string/char.h"
#include "core/core_base.h"

class String;

class CORE_API NumberConverter
{
public:
   static NumberConverter& getInstance();

 // conversion
   int toInt(UChar value);
   int toInt(const String& value);
   float toFloat(const String& value);
   double toDouble(const String& value);

   String& format(String& result, int value);
   String& format(String& result, unsigned int value);
   String& format(String& result, double value);

private:
   NumberConverter() = default;
};

#endif // NUMBER_CONVERTER_H
