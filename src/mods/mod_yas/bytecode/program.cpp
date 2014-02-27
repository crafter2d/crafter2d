
#include "program.h"

namespace ByteCode
{
   Program::Program():
      mSymbolTable(),
      mStringCache()
   {
   }

   // - Get/set

   const SymbolTable& Program::getSymbolTable() const
   {
      return mSymbolTable;
   }

   SymbolTable& Program::getSymbolTable()
   {
      return mSymbolTable;
   }
   
   const StringCache& Program::getStringCache() const
   {
      return mStringCache;
   }
            
   StringCache& Program::getStringCache()
   {
      return mStringCache;
   }

}