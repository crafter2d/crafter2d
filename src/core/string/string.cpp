
#include "string.h"

#include <unicode/unistr.h>

class StringData
{
public:
   StringData(): mString() {}
   StringData(const StringData& that): mString(that.mString) {}
   StringData(const char* pdata): mString(pdata) {}

   UnicodeString mString;
};

String::String():
   mpData(new StringData())
{
}

String::String(const String& that):
   mpData(new StringData(*that.mpData))
{
}

String::String(const char* pdata):
   mpData(new StringData(pdata))
{
}

String::~String()
{
   delete mpData;
}

const String& String::operator=(const String& that)
{
   return *this;
}

// - Query

int String::length() const
{
   return mpData->mString.length();
}

char* String::toUtf8(int& length) const
{
   length = mpData->mString.length();
   char* pdata = new char[length];

   CheckedArrayByteSink sink(pdata, length);
   mpData->mString.toUTF8(sink);

   return pdata;
}

// - Operations

const String& String::toLower()
{
   mpData->mString.toLower();
   return *this;
}

const String& String::toUpper()
{
   mpData->mString.toUpper();
   return *this;
}

// - Conversion

std::string String::toStdString() const
{
   UErrorCode status = U_ZERO_ERROR;
   char* pdata = 0;

   int32_t ln = mpData->mString.extract(pdata, 0, NULL, status);
   pdata = new char[ln+1];
   status = U_ZERO_ERROR;
   mpData->mString.extract(pdata, ln, NULL, status);
   if ( status != U_ZERO_ERROR )
   {
      // bah
   }

   pdata[ln] = 0;
   std::string result = pdata;
   delete[] pdata;
   
   return result;
}