
#ifndef CIL_SWITCHTABLE_ENTRY_H
#define CIL_SWITCHTABLE_ENTRY_H

#include "core/system/variant.h"

namespace CIL
{
   class SwitchTableEntry
   {
   public:
      SwitchTableEntry();
      SwitchTableEntry(const SwitchTableEntry& that);

      const SwitchTableEntry operator=(const SwitchTableEntry& that);

      int      label;
      Variant  value;
   };
}

#endif // CIL_SWITCHTABLE_ENTRY_H
