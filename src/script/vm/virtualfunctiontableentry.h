
#ifndef VIRTUAL_FUNCTION_TABLE_ENTRY_H_
#define VIRTUAL_FUNCTION_TABLE_ENTRY_H_

#include "core/string/string.h"

#include "virtualguard.h"

class VirtualFunctionTableEntry
{
public:
   typedef std::vector<VM::Guard> Guards;

   VirtualFunctionTableEntry():
      mName(),
      guards(),
      mInstruction(-1),
      mArguments(0),
      mInterface(-1),
      returns(false)
   {
   }

   VirtualFunctionTableEntry(const VirtualFunctionTableEntry& that): 
      mName(that.mName),
      guards(that.guards),
      mInstruction(that.mInstruction),
      mArguments(that.mArguments),
      mInterface(that.mInterface),
      returns(that.returns)
   {
   }

   VirtualFunctionTableEntry* clone() const {
      return new VirtualFunctionTableEntry(*this);
   }

   String mName;
   Guards guards;
   int    mInstruction;
   int    mArguments;
   int    mInterface;
   bool   returns;
};

#endif // VIRTUAL_FUNCTION_TABLE_ENTRY_H_
