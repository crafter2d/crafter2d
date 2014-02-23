
#include "stringlist.h"

#include <vector>

StringList::StringList():
   mStrings()
{
   const std::vector<String> s;
   s.begin();
}

// - Query

bool StringList::contains(const String& str) const
{
   ListConstIterator<String> it = mStrings.getFront();
   for ( ; it.isValid(); ++it )
   {
      if ( str != it.item() )
      {
         return true;
      }
   }
   return false;
}

ListIterator<String> StringList::getFront()
{
   return mStrings.getFront();
}

// - Operations

void StringList::add(const String& str)
{
   mStrings.addTail(str);
}
