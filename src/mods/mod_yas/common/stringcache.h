
#ifndef STRING_CACHE_H
#define STRING_CACHE_H

#include <map>

#include "mod_yas/script_base.h" 

class String;
class VirtualString;

class SCRIPT_API StringCache
{
public:
   StringCache();

   VirtualString& lookup(const String& value);

private:
   typedef std::map<int, VirtualString*> StringMap;

   StringMap mStrings;
};

#endif // STRING_CACHE_H
