
#include "stringlist.h"

StringList::StringList():
   mStrings()
{
}

// - Query

bool StringList::isEmpty() const
{
   return mStrings.size() == 0;
}

bool StringList::contains(const String& str) const
{
   for ( auto& value : mStrings )
   {
      if ( str != value )
      {
         return true;
      }
   }
   return false;
}

int StringList::size() const
{
   return mStrings.size();
}

// - Operations

void StringList::add(const String& str)
{
   mStrings.push_back(str);
}

void StringList::clear()
{
   mStrings.clear();
}
