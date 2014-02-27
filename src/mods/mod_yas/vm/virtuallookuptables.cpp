
#include "virtuallookuptables.h"

VirtualLookupTables::~VirtualLookupTables()
{
   clear();
}

const VirtualLookupTable& VirtualLookupTables::operator[](int index) const
{
   return *mTables[index];
}
         
VirtualLookupTable& VirtualLookupTables::operator[](int index)
{
   return *mTables[index];
}

// - Query

int VirtualLookupTables::size() const
{
   return mTables.size();
}

// - Operations

void VirtualLookupTables::add(VirtualLookupTable* ptable)
{
   mTables.push_back(ptable);
}

void VirtualLookupTables::clear()
{
   for ( std::size_t index = 0; index < mTables.size(); ++index )
   {
      VirtualLookupTable* ptable = mTables[index];
      delete ptable;
   }
   mTables.clear();
}
