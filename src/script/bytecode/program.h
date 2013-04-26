
#ifndef BYTECODE_PROGRAM_H
#define BYTECODE_PROGRAM_H

#include "symboltable.h"

namespace ByteCode
{
   class Program
   {
   public:
      
    // get/set
      const SymbolTable& getSymbolTable() const;
            SymbolTable& getSymbolTable();

   private:

      SymbolTable mSymbolTable;
   };
}

#endif // BYTECODE_PROGRAM_H
