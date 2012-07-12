
#include "string.h"

#include <string.h>

String::String():
   mString()
{
}

String::String(const char* pdata):
   mString(pdata)
{
}

String::String(const std::string& that):
   mString(that.c_str())
{
}

String::String(const String& that):
   mString(that.mString)
{
}

String::~String()
{
}

const String& String::operator=(const String& that)
{
   mString = that.mString;
   return *this;
}

const String& String::operator=(const char* pstring)
{
   mString = pstring;
   return *this;
}

const String& String::operator=(const std::string& that)
{
   mString = that.c_str();
   return *this;
}

bool String::operator<=(const String& that) const
{
   return mString.compare(that.mString) <= 0;
}

bool String::operator<(const String& that) const
{
   return mString.compare(that.mString) < 0;
}

bool String::operator>(const String& that) const
{
   return mString.compare(that.mString) > 0;
}

bool String::operator>=(const String& that) const
{
   return mString.compare(that.mString) >= 0;
}

const String& String::operator+=(char c)
{
   mString += c;
   return *this;
}

const String& String::operator+=(const String& that)
{
   mString += that.mString;
   return *this;
}

String String::operator+(const String& that)
{
   String result;
   result.mString = mString + that.mString;
   return result;
}

// - Query

int String::length() const
{
   return mString.length();
}

char* String::toUtf8(int& length) const
{
   length = mString.length();
   char* pdata = new char[length+1];
   memset(pdata, 0, length + 1);

   CheckedArrayByteSink sink(pdata, length);
   mString.toUTF8(sink);

   return pdata;
}

int String::compare(const String& that) const
{
   return mString.compare(that.mString);
}

// - Comparison

bool String::operator==(const String& that) const
{
   return mString == that.mString;
}

// - Operations

const String& String::toLower()
{
   mString.toLower();
   return *this;
}

const String& String::toUpper()
{
   mString.toUpper();
   return *this;
}

const String& String::trim()
{
   mString.trim();
   return *this;
}

void String::replace(int original, int newtext)
{
   mString.findAndReplace(original, newtext);
}

String String::subStr(int start, int count) const
{
   String result;
   mString.extract(start, count, result.mString);
   return result;
}

String String::unescape() const
{
   String result;
   result.mString = mString.unescape();
   return result;
}

// - Searching

int String::indexOf(char character)
{
   return mString.indexOf(character);
}

// - Conversion

std::string String::toStdString() const
{
   UErrorCode status = U_ZERO_ERROR;
   char* pdata = 0;

   int32_t ln = mString.extract(pdata, 0, NULL, status);
   pdata = new char[ln+1];
   status = U_ZERO_ERROR;
   mString.extract(pdata, ln, NULL, status);
   if ( status != U_ZERO_ERROR )
   {
      // bah
   }

   pdata[ln] = 0;
   std::string result = pdata;
   delete[] pdata;

   return result;
}
