
#include "stringcache.h"

#include "core/string/string.h"

#include "script/vm/virtualstring.h"

StringCache::StringCache()
{
}

// - Lookup

VirtualString& StringCache::lookup(const String& value)
{
   int hash = value.hashCode();
   StringMap::iterator it = mStrings.find(hash);
   if ( it == mStrings.end() )
   {
      VirtualString* pstring = new VirtualString(value);
      mStrings.insert(std::pair<int, VirtualString*>(hash, pstring));
   }

   return *mStrings[hash];
}
