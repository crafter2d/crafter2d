
#include "functiontable.h"

#include <algorithm>

#include "function.h"
#include "signature.h"

namespace CIL
{
   FunctionTable::FunctionTable():
      mFunctions()
   {
   }

   const FunctionTable& FunctionTable::operator=(const FunctionTable& that)
   {
      mFunctions = that.mFunctions;
      return *this;
   }

   // - Query

   int FunctionTable::size() const
   {
      return mFunctions.size();
   }

   bool FunctionTable::contains(const Function& function) const
   {
      return std::find(mFunctions.begin(), mFunctions.end(), &function) != mFunctions.end();
   }

   const Function& FunctionTable::operator[](int index) const
   {
      return *mFunctions[index];
   }

   Function& FunctionTable::operator[](int index)
   {
      return *mFunctions[index];
   }

   // - Operations
   
   void FunctionTable::insert(Function& function)
   {
      for ( std::size_t index = 0; index < mFunctions.size(); index++ )
      {
         Function* pfunction = mFunctions[index];

         if ( pfunction->getName() == function.getName()
           && pfunction->getSignature().match(function.getSignature()) ) // for now do not allow derived types
         {
            mFunctions[index] = &function;
            return;
         }
      }

      mFunctions.push_back(&function);
   }

   void FunctionTable::insert(FunctionTable& table)
   {
      for ( int index = 0; index < table.size(); index++ )
      {
         insert(table[index]);
      }
   }

   void FunctionTable::reindex()
   {
      int resourceindex = 2;

      for ( std::size_t index = 0; index < mFunctions.size(); index++ )
      {
         Function* pfunction = mFunctions[index];
         pfunction->setId(resourceindex++); // add static & var init blocks
      }
   }
}