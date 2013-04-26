
#include "program.h"

namespace ByteCode
{

   // - Get/set

   const SymbolTable& Program::getSymbolTable() const
   {
      return mSymbolTable;
   }

   SymbolTable& Program::getSymbolTable()
   {
      return mSymbolTable;
   }

}