
#ifndef BYTECODE_PROGRAM_H
#define BYTECODE_PROGRAM_H

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

      int         getSize() const;
      const char* getCode() const;

    // operations
      int linkCode(const char* pcode, unsigned int size);

   private:
      void allocateCode(int size);

      SymbolTable    mSymbolTable;
      char*          mpCode;
      unsigned int   mCodeSize;
      unsigned int   mCodeLength;
   };
}

#endif // BYTECODE_PROGRAM_H
