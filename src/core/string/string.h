
#ifndef STRING_H_
#define STRING_H_

#include <string>

#include "core/core_base.h"

class StringData;

class CORE_API String
{
public:
            String();
            String(const String& that);
   explicit String(const char* pdata);
   ~String();

   const String& operator=(const String& that);
   const String& operator=(const char* pstring);

   bool operator==(const String& that) const;

 // query
   int length() const;
   char* toUtf8(int& length) const;

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
   StringData* mpData;
};

#endif // STRING_H_
