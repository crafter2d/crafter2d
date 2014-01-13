
#include "virtualfunctiontable.h"

#include "core/defines.h"
#include "core/string/string.h"

#include "virtualfunctiontableentry.h"

VirtualFunctionTable::VirtualFunctionTable():
   mEntries()
{
}

VirtualFunctionTable::~VirtualFunctionTable()
{
   clear();
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

void VirtualFunctionTable::append(VirtualFunctionTableEntry* pentry)
{
   mEntries.push_back(pentry);
}

void VirtualFunctionTable::append(const VirtualFunctionTable& vtable)
{
   for ( std::size_t index = 2; index < vtable.mEntries.size(); index++ )
   {
      VirtualFunctionTableEntry* pentry = mEntries[index];
      append(pentry->clone());
   }
}

void VirtualFunctionTable::clear()
{
   for ( std::size_t index = 0; index < mEntries.size(); index++ )
   {
      delete mEntries[index];
   }
}

void VirtualFunctionTable::merge(const VirtualFunctionTable& that)
{
   ASSERT(that.mEntries.size() <= mEntries.size());

   for ( std::size_t index = 2; index < that.mEntries.size(); index++ )
   {
      VirtualFunctionTableEntry* pentry = mEntries[index];
      if ( pentry->mInstruction == -1 )
      {
         pentry->mInstruction = that.mEntries[index]->mInstruction;
      }
      if ( pentry->mInterface == -1 )
      {
         pentry->mInterface = that.mEntries[index]->mInterface;
      }
   }
}

// - Search

const VirtualFunctionTableEntry* VirtualFunctionTable::findByName(const String& name) const
{
   for ( std::size_t index = 0; index < mEntries.size(); index++ )
   {
      const VirtualFunctionTableEntry* pentry = mEntries[index];
      if ( pentry->mName == name )
      {
         return pentry;
      }
   }
   return NULL;
}

const VirtualFunctionTableEntry& VirtualFunctionTable::resolveInterface(int interfaceid) const
{
   for ( std::size_t index = 0; index < mEntries.size(); index++ )
   {
      const VirtualFunctionTableEntry* pentry = mEntries[index];
      if ( pentry->mInterface == interfaceid )
      {
         return *pentry;
      }
   }
   
   throw std::exception();
}
