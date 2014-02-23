
#ifndef BYTECODE_VALUESYMBOL_H
#define BYTECODE_VALUESYMBOL_H

#include "mod_yas/vm/virtualvalue.h"

#include "symbol.h"

namespace ByteCode
{
   class ValueSymbol : public Symbol
   {
   public:
      VirtualValue value;
   };
}

#endif // BYTECODE_VALUESYMBOL_H
