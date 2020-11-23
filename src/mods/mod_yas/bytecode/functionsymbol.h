
#ifndef BYTECODE_FUNCTIONSYMBOL_H
#define BYTECODE_FUNCTIONSYMBOL_H

#include "core/string/string.h"

#include "symbol.h"

namespace ByteCode
{
   class FunctionSymbol : public Symbol
   {
   public:
      virtual ~FunctionSymbol() = default;

      String klass;
      int func;
      int args;
      bool returns;
   };
}

#endif // BYTECODE_FUNCTIONSYMBOL_H
