
#ifndef STRING_H_
#define STRING_H_

#include <unicode/unistr.h>
#include <string>

#include "core/core_base.h"

class CORE_API String
{
public:
            String();
            String(const String& that);
   explicit String(const std::string& that);
   explicit String(const char* pdata);
   ~String();

   const String& operator=(const String& that);
   const String& operator=(const char* pstring);
   const String& operator=(const std::string& that);

   bool operator<=(const String& that) const;
   bool operator<(const String& that) const;
   bool operator>(const String& that) const;
   bool operator>=(const String& that) const;

   bool operator==(const String& that) const;

   const String& operator+=(char c);
   const String& operator+=(const String& that);
   String operator+(const String& that);

 // query
   int length() const;
   char* toUtf8(int& length) const;
   int compare(const String& that) const;

 // operations
   const String& toLower();
   const String& toUpper();
   const String& trim();

   void replace(int original, int newtext);

   String subStr(int start, int count) const;
   String unescape() const;

 // searching
   int indexOf(char character);

 // conversion
   std::string toStdString() const;

private:
   friend class NumberConverter;

   UnicodeString mString;
};

#endif // STRING_H_
