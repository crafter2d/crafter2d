
#include "stringlist.h"

#include <vector>

StringList::StringList():
   mStrings()
{
   const std::vector<String> s;
   s.begin();
}

// - Query

bool StringList::isEmpty() const
{
   return size() == 0;
}

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

int StringList::size() const
{
   return mStrings.size();
}

ListIterator<String> StringList::getFront()
{
   return mStrings.getFront();
}

ListConstIterator<String> StringList::getFront() const
{
   return mStrings.getFront();
}

// - Operations

void StringList::add(const String& str)
{
   mStrings.addTail(str);
}

void StringList::clear()
{
   mStrings.clear();
}
