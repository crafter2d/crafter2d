
#ifndef BYTECODE_NATIVEFUNCTIONSYMBOL_H
#define BYTECODE_NATIVEFUNCTIONSYMBOL_H

#include "core/string/string.h"

#include "mod_yas/common/functionregistration.h"

#include "symbol.h"

namespace ByteCode
{
   class NativeFunctionSymbol : public Symbol
   {
   public:
      FunctionRegistration* pnativefunction;
      int  args;
      bool returns;
   };
}

#endif // BYTECODE_FUNCTIONSYMBOL_H

