
#ifndef BYTECODE_FUNCTIONSYMBOL_H
#define BYTECODE_FUNCTIONSYMBOL_H

#include "symbol.h"

namespace ByteCode
{
   class FunctionSymbol : public Symbol
   {
   public:
      VirtualClass* klass;
      int func;
      int args;
      bool returns;
   };
}

#endif // BYTECODE_FUNCTIONSYMBOL_H
