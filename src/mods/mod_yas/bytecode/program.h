
#ifndef BYTECODE_PROGRAM_H
#define BYTECODE_PROGRAM_H

#include "mod_yas/common/stringcache.h"

#include "symboltable.h"

namespace ByteCode
{
   class Program
   {
   public:
      Program();
      
    // get/set
      const SymbolTable& getSymbolTable() const;
            SymbolTable& getSymbolTable();

      const StringCache& getStringCache() const;
            StringCache& getStringCache();

   private:
    // data
      SymbolTable       mSymbolTable;
      StringCache       mStringCache;
   };
}

#endif // BYTECODE_PROGRAM_H
