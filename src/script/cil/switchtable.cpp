
#include "switchtabel.h" 

#include "script/common/variant.h"

#include "switchtableentry.h"

namespace CIL
{

   SwitchTable::SwitchTable():
      mCases(),
      mDefault(-1),
      mEnd(-1)
   {
   }

   // operators

   const SwitchTableEntry& SwitchTable::operator[](int index) const
   {
      return mCases[index];
   }

   SwitchTableEntry& SwitchTable::operator[](int index)
   {
      return mCases[index];
   }

    // - Query
      
   int SwitchTable::size() const
   {
      return mCases.size();
   }

   bool SwitchTable::hasDefault() const
   {
      return mDefault != -1;
   }
      
   int SwitchTable::getDefault() const
   {
      return mDefault;
   }

   int SwitchTable::getEnd() const
   {
      return mEnd;
   }

   // - Maintenance

   void SwitchTable::add(const SwitchTableEntry& entry)
   {
      mCases.push_back(entry);
   }
      
   void SwitchTable::addDefault(int label)
   {
      mDefault = label;
   }

   void SwitchTable::addEnd(int label)
   {
      mEnd = label;
   }
}
