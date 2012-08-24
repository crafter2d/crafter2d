
#include "virtuallookuptable.h"

VirtualLookupTable::VirtualLookupTable():
   mEntries(),
   mDefault(-1),
   mEnd(-1)
{
}

// - Get/set

void VirtualLookupTable::setDefault(int codeindex)
{
   mDefault = codeindex;
}

void VirtualLookupTable::setEnd(int codeindex)
{
   mEnd = codeindex;
}

// - Query

int VirtualLookupTable::lookup(const Variant& value) const
{
   EntryMap::const_iterator it = mEntries.find(value);
   if ( it != mEntries.end() )
   {
      Entry* pentry = it->second;
      return pentry->mActual;
   }
   else if ( mDefault >= 0 )
   {
      return mDefault;
   }

   return mEnd;
}

// - Operations
   
void VirtualLookupTable::add(const Variant& variant, int codeindex)
{
   Entry* pentry = new Entry(codeindex);
   mEntries[variant] = pentry;
}

void VirtualLookupTable::offsetCode(int offset)
{
   EntryMap::iterator it = mEntries.begin();
   for ( ; it != mEntries.end(); ++it )
   {
      Entry* pentry = it->second;
      pentry->mActual += offset;
   }

   if ( mDefault != -1 )
      mDefault += offset;

   if ( mEnd != -1 )
      mEnd += offset;
}
