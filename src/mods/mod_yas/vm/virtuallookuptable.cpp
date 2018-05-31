
#include "virtuallookuptable.h"

#include <algorithm>

#include "virtualvalue.h"

VirtualLookupTable::VirtualLookupTable():
   mEntries(),
   mDefault(-1),
   mEnd(-1)
{
}

// - Get/set

bool VirtualLookupTable::hasDefault() const
{
   return mDefault != -1;
}

int VirtualLookupTable::getDefault() const
{
   return mDefault;
}

void VirtualLookupTable::setDefault(int codeindex)
{
   mDefault = codeindex;
}

int VirtualLookupTable::getEnd() const
{
   return mEnd;
}

void VirtualLookupTable::setEnd(int codeindex)
{
   mEnd = codeindex;
}

// - Query

int VirtualLookupTable::lookup(const VirtualValue& value) const
{
   EntryMap::const_iterator it = mEntries.find(value);
   if ( it != mEntries.end() )
   {
      return it->second;
   }
   else if ( mDefault != -1 )
   {
      return mDefault;
   }

   return mEnd;
}

const VirtualValue* VirtualLookupTable::lookup(int pos) const
{
   EntryMap::const_iterator it = mEntries.begin();
   for ( ; it != mEntries.end(); ++it )
   {
      if ( it->second == pos )
      {
         return &it->first;
      }
   }
   return nullptr;
}

std::vector<int> VirtualLookupTable::getPositions() const
{
   std::vector<int> entries;
   EntryMap::const_iterator it = mEntries.begin();
   for ( ; it != mEntries.end(); ++it )
   {
      entries.push_back(it->second);
   }
   std::sort(entries.begin(), entries.end());
   return entries;
}

// - Operations
   
void VirtualLookupTable::add(const VirtualValue& variant, int codeindex)
{
   mEntries[variant] = codeindex;
}

void VirtualLookupTable::updatePosition(int offset)
{
   EntryMap::iterator it = mEntries.begin();
   for ( ; it != mEntries.end(); ++it )
   {
      it->second += offset;
   }

   if ( mDefault != -1 )
      mDefault += offset;

   if ( mEnd != -1 )
      mEnd += offset;
}
