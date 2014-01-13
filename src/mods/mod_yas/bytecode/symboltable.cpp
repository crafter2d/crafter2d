
#include "symboltable.h"

#include "symbol.h"

namespace ByteCode
{
   SymbolTable::SymbolTable():
      mSymbols()
   {
   }

   SymbolTable::~SymbolTable()
   {
      for ( std::size_t index = 0; index < mSymbols.size(); ++index )
      {
         delete mSymbols[index];
      }
      mSymbols.clear();
   }

   const Symbol& SymbolTable::operator[](int index) const
   {
      return *mSymbols[index];
   }

   int SymbolTable::add(Symbol* psymbol)
   {
      mSymbols.push_back(psymbol);
      return mSymbols.size() - 1;
   }

}