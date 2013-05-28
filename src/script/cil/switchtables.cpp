
#include "switchtables.h"

namespace CIL
{
   const SwitchTables& SwitchTables::operator=(const SwitchTables& that)
   {
      mTables = that.mTables;
      return *this;
   }

   const SwitchTable& SwitchTables::operator[](int index) const
   {
      return *mTables[index];
   }
            
   SwitchTable& SwitchTables::operator[](int index)
   {
      return *mTables[index];
   }

   // - Query
      
   int SwitchTables::size() const
   {
      return mTables.size();
   }

   // - Maintanance

   int SwitchTables::add(SwitchTable* ptable)
   {
      mTables.push_back(ptable);
      return mTables.size() - 1;
   }

   void SwitchTables::clear()
   {
      mTables.clear();
   }
}
