
#include "virtualfunctiontable.h"

#include "core/defines.h"
#include "core/string/string.h"

#include "virtualfunction.h"
#include "virtualfunctiontableentry.h"

VirtualFunctionTable::VirtualFunctionTable():
   mEntries()
{
}

VirtualFunctionTable::~VirtualFunctionTable()
{
   clear();
}

const VirtualFunctionTable& VirtualFunctionTable::operator=(const VirtualFunctionTable& that)
{
   for ( std::size_t index = 0; index < that.mEntries.size(); ++index )
   {
      mEntries.push_back(that.mEntries[index]->clone());
   }
   return *this;
}

// - Query

int VirtualFunctionTable::size() const
{ 
   return mEntries.size();
}

const VirtualFunctionTableEntry& VirtualFunctionTable::operator[](int index) const 
{
   return *mEntries[index];
}

VirtualFunctionTableEntry& VirtualFunctionTable::operator[](int index)
{
   return *mEntries[index];
}

// - Operations

void VirtualFunctionTable::setInits(VirtualFunction& static_init, VirtualFunction& var_init)
{
   if ( mEntries.empty() )
   {
      mEntries.push_back(VirtualFunctionTableEntry::fromFunction(static_init));
      mEntries.push_back(VirtualFunctionTableEntry::fromFunction(var_init));
   }
   else
   {
      mEntries[0]->fromFunction(static_init);
      mEntries[1]->fromFunction(var_init);
   }

   static_init.setIndex(0);
   var_init.setIndex(1);
}

int VirtualFunctionTable::insert(VirtualFunction& function)
{
   // insert a function in the v-table, we can skip the first 2

   for ( std::size_t index = 2; index < mEntries.size(); ++index )
   {
      VirtualFunctionTableEntry* pentry = mEntries[index];
      if ( pentry->mpFunction->equals(function) )
      {
         pentry->setFunction(function);
         return index;
      }
   }

   VirtualFunctionTableEntry* pentry = VirtualFunctionTableEntry::fromFunction(function);
   mEntries.push_back(pentry);
   return mEntries.size() - 1;
}

void VirtualFunctionTable::clear()
{
   for ( std::size_t index = 0; index < mEntries.size(); index++ )
   {
      delete mEntries[index];
   }
}

void VirtualFunctionTable::update()
{
   for ( std::size_t index = 0; index < mEntries.size(); index++ )
   {
      mEntries[index]->update();
   }
}

// - Search

const VirtualFunctionTableEntry* VirtualFunctionTable::findByName(const String& name) const
{
   for ( std::size_t index = 0; index < mEntries.size(); index++ )
   {
      const VirtualFunctionTableEntry* pentry = mEntries[index];
      if ( pentry->getName() == name )
      {
         return pentry;
      }
   }
   return NULL;
}
