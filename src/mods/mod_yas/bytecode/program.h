
#ifndef BYTECODE_PROGRAM_H
#define BYTECODE_PROGRAM_H

#include "mod_yas/common/stringcache.h"

#include "symboltable.h"

namespace ByteCode
{
   class IRGenerator;

   class Program
   {
   public:
      Program();
      
    // get/set
      const SymbolTable& getSymbolTable() const;
            SymbolTable& getSymbolTable();

      const StringCache& getStringCache() const;
            StringCache& getStringCache();

      IRGenerator& getByteCodeGenerator() const;
      void         setByteCodeGenerator(IRGenerator* pgenerator);

      int         getSize() const;
      const char* getCode() const;

    // operations
      int linkCode(const char* pcode, unsigned int size);

   private:
      void allocateCode(int size);

      SymbolTable       mSymbolTable;
      StringCache       mStringCache;
      IRGenerator*      mpIRGenerator;
      char*             mpCode;
      unsigned int      mCodeSize;
      unsigned int      mCodeLength;
   };
}

#endif // BYTECODE_PROGRAM_H
