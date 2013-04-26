
#ifndef BYTECODE_VALUESYMBOL_H
#define BYTECODE_VALUESYMBOL_H

#include "script/common/variant.h"

#include "symbol.h"

namespace ByteCode
{
   class ValueSymbol : public Symbol
   {
   public:
      Variant value;
   };
}

#endif // BYTECODE_VALUESYMBOL_H
