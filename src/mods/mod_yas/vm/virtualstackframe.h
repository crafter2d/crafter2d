
#ifndef VM_STACKFRAME_H
#define VM_STACKFRAME_H

#include <vector>

#include "virtualvalue.h"

class VirtualClass;
class VirtualFunctionTableEntry;

namespace VM
{
   class Guard;

   class StackFrame
   {
   public:
      typedef std::vector<VirtualValue> Locals;

      StackFrame():
         pclass(nullptr),
         pentry(nullptr),
         locals(),
         sp(0),
         retaddress(-1),
         retcode(nullptr),
         callnative(false)
      {
      }

      const VirtualClass*              pclass;
      const VirtualFunctionTableEntry* pentry;

      Locals                           locals;

      int                              sp;
      int                              retaddress;
      const char*                      retcode;

      bool                             callnative;

   };
}

#endif // VM_STACKFRAME_H
