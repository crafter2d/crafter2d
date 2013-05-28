
#ifndef VIRTUAL_FUNCTION_TABLE_ENTRY_H_
#define VIRTUAL_FUNCTION_TABLE_ENTRY_H_

#include "core/string/string.h"

#include "virtualguards.h"

class VirtualFunctionTableEntry
{
public:
   VirtualFunctionTableEntry():
      mName(),
      guards(),
      mInstruction(-1),
      mArguments(0),
      mLocals(0),
      mInterface(-1),
      returns(false)
   {
   }

   VirtualFunctionTableEntry(const VirtualFunctionTableEntry& that): 
      mName(that.mName),
      guards(that.guards),
      mInstruction(that.mInstruction),
      mArguments(that.mArguments),
      mLocals(that.mLocals),
      mInterface(that.mInterface),
      returns(that.returns)
   {
   }

   VirtualFunctionTableEntry* clone() const {
      return new VirtualFunctionTableEntry(*this);
   }

   void addGuard(VirtualGuard* pguard) {
      guards.add(pguard);
   }

   void updateGuards() {
      guards.updatePosition(mInstruction);
   }

   String         mName;
   VirtualGuards  guards;
   int            mInstruction;
   int            mArguments;
   int            mLocals;
   int            mInterface;
   bool           returns;
};

#endif // VIRTUAL_FUNCTION_TABLE_ENTRY_H_
