
#ifndef VM_STACKFRAME_H
#define VM_STACKFRAME_H

#include <vector>

#include "script/common/variant.h"

class VirtualClass;
class VirtualFunctionTableEntry;

namespace VM
{

   class StackFrame
   {
   public:
      typedef std::vector<Variant> Locals;

      const VirtualClass*              pclass;
      const VirtualFunctionTableEntry* pentry;

      Locals                           locals;

      int                              retaddress;

   };
}

#endif // VM_STACKFRAME_H
