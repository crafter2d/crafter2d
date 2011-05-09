
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
   String(const char* pdata);
   ~String();

   const String& operator=(const String& that);

 // query
   int length() const;
   char* toUtf8(int& length) const;

 // operations
   const String& toLower();
   const String& toUpper();

 // conversion
   std::string toStdString() const;

private:
   StringData* mpData;
};

#endif // STRING_H_
