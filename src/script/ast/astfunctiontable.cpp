
#include "astfunctiontable.h"

#include <algorithm>

#include "script/ast/astfunction.h"
#include "script/ast/astfunctionmap.h"
#include "script/ast/astsignature.h"

ASTFunctionTable::ASTFunctionTable():
   mFunctions()
{
   // reserve space for the initializer functions
   mFunctions.push_back(NULL);
   mFunctions.push_back(NULL);
}

const ASTFunctionTable& ASTFunctionTable::operator=(const ASTFunctionTable& that)
{
   mFunctions = that.mFunctions;
   return *this;
}

// - Query

int ASTFunctionTable::size() const
{
   return mFunctions.size();
}

bool ASTFunctionTable::contains(const ASTFunction& function) const
{
   return std::find(mFunctions.begin(), mFunctions.end(), &function) != mFunctions.end();
}

const ASTFunction& ASTFunctionTable::operator[](int index) const
{
   return *mFunctions[index];
}

ASTFunction& ASTFunctionTable::operator[](int index)
{
   return *mFunctions[index];
}

// - Operations

void ASTFunctionTable::build(ASTFunctionMap& functions)
{
   // we assume that the base class table is already assigned
   // now insert local functions into the table

   ASTFunctionMap::Iterator it = functions.getIterator();
   while ( functions.hasNext(it) )
   {
      ASTFunction& function = functions.getNext(it);
      insert(function);
   }
   
   reindex();
}

void ASTFunctionTable::set(int index, ASTFunction& function)
{
   mFunctions[index] = &function;
}
   
void ASTFunctionTable::insert(ASTFunction& function)
{
   for ( std::size_t index = 2; index < mFunctions.size(); index++ )
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

void ASTFunctionTable::insert(ASTFunctionTable& table)
{
   for ( int index = 0; index < table.size(); index++ )
   {
      insert(table[index]);
   }
}

void ASTFunctionTable::reindex()
{
   int resourceindex = 2;

   for ( std::size_t index = 2; index < mFunctions.size(); index++ )
   {
      ASTFunction* pfunction = mFunctions[index];
      if ( !pfunction->getModifiers().isPureNative() )
      {
         pfunction->setResourceIndex(resourceindex++); // add static & var init blocks
      }
   }
}
