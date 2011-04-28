
#include "functiontable.h"

#include <algorithm>

#include "../ast/astfunction.h"

#include "signature.h"

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

bool FunctionTable::contains(const ASTFunction& function) const
{
   return std::find(mFunctions.begin(), mFunctions.end(), &function) != mFunctions.end();
}

const ASTFunction& FunctionTable::operator[](int index) const
{
   return *mFunctions[index];
}

ASTFunction& FunctionTable::operator[](int index)
{
   return *mFunctions[index];
}

// - Operations
   
void FunctionTable::insert(ASTFunction& function)
{
   for ( std::size_t index = 0; index < mFunctions.size(); index++ )
   {
      ASTFunction* pfunction = mFunctions[index];

      if ( pfunction->getName() == function.getName()
        && pfunction->getSignature().exactMatch(function.getSignature()) ) // for now do not allow derived types
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
   for ( std::size_t index = 0; index < mFunctions.size(); index++ )
   {
      ASTFunction* pfunction = mFunctions[index];
      pfunction->setResourceIndex(index+2); // add static & var init blocks
   }
}
