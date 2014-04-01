
#include "switchtableentry.h"

namespace CIL
{
   SwitchTableEntry::SwitchTableEntry() :
      label(-1),
      value()
   {
   }

   SwitchTableEntry::SwitchTableEntry(const SwitchTableEntry& that) :
      label(that.label),
      value(that.value)
   {
   }

   const SwitchTableEntry SwitchTableEntry::operator=(const SwitchTableEntry& that)
   {
      label = that.label;
      value = that.value;
      return *this;
   }
}
