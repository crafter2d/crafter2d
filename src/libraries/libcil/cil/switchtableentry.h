
#ifndef CIL_SWITCHTABLE_ENTRY_H
#define CIL_SWITCHTABLE_ENTRY_H

#include "core/system/variant.h"

namespace CIL
{
   class SwitchTableEntry
   {
   public:
      SwitchTableEntry() : label(0), value() {}

      int      label;
      Variant  value;
   };
}

#endif // CIL_SWITCHTABLE_ENTRY_H
