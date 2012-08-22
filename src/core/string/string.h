
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
            String(const char* pdata);
           ~String();

   const char operator[](int index) const;
         char operator[](int index);

   const String& operator=(const String& that);
   const String& operator=(const char* pstring);
   const String& operator=(const std::string& that);

   bool operator<=(const String& that) const;
   bool operator<(const String& that) const;
   bool operator>(const String& that) const;
   bool operator>=(const String& that) const;

   bool operator==(const String& that) const;
   bool operator!=(const String& that) const;

   const String& operator+=(char c);
   const String& operator+=(const String& that);
   String operator+(const String& that) const;
   String operator+(char c) const;

 // query
   bool isEmpty() const;
   int length() const;
   const char* getBuffer() const;
   char* toUtf8(int& length) const;
   int compare(const String& that) const;
   int hashCode() const;

 // operations
   const String& toLower();
   const String& toUpper();
   const String& trim();

   void append(const String& that);
   void setTo(const char* ptext, int length);

   void replace(char original, char newtext);
   void replace(int start, int length, const String& with);
   void remove(int start, int count);

   String subStr(int start, int count) const;
   String unescape() const;

 // searching
   int indexOf(char character, int start = 0) const;
   int lastIndexOf(char character) const;

 // conversion
   std::string toStdString() const;

private:
   friend class NumberConverter;

   UnicodeString mString;
};

#endif // STRING_H_
