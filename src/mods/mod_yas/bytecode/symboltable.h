
#ifndef BYTECODE_SYMBOLTABLE_H
#define BYTECODE_SYMBOLTABLE_H

#include <vector>

namespace ByteCode
{
   class Symbol;

   class SymbolTable
   {
      typedef std::vector<Symbol*> Symbols;

   public:
      SymbolTable();
      ~SymbolTable();

      const Symbol& operator[](int index) const;

    // maintenance
      int add(Symbol* psymbol);

   private:

      Symbols mSymbols;
   };
}

#endif // BYTECODE_SYMBOLTABLE_H
