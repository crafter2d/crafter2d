
#ifndef VM_STACKFRAME_H
#define VM_STACKFRAME_H

#include <vector>

#include "script/common/variant.h"

class VirtualClass;
class VirtualFunctionTableEntry;

namespace VM
{
   class Guard;

   class StackFrame
   {
   public:
      typedef std::vector<Variant> Locals;

      StackFrame():
         pclass(NULL),
         pentry(NULL),
         locals(),
         sp(0),
         retaddress(-1),
         callnative(false)
      {
      }

      const VirtualClass*              pclass;
      const VirtualFunctionTableEntry* pentry;

      Locals                           locals;

      int                              sp;
      int                              retaddress;

      bool                             callnative;

   };
}

#endif // VM_STACKFRAME_H
