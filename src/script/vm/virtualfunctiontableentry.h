
#ifndef VIRTUAL_FUNCTION_TABLE_ENTRY_H_
#define VIRTUAL_FUNCTION_TABLE_ENTRY_H_

#include "core/string/string.h"

class VirtualFunctionTableEntry
{
public:
   VirtualFunctionTableEntry():
      mName(),
      mpCode(NULL),
      mInstruction(-1),
      mArguments(0),
      mInterface(-1)
   {
   }

   VirtualFunctionTableEntry(const VirtualFunctionTableEntry& that): 
      mName(that.mName),
      mInstruction(that.mInstruction),
      mArguments(that.mArguments),
      mInterface(that.mInterface)
   {
   }

   VirtualFunctionTableEntry* clone() const {
      return new VirtualFunctionTableEntry(*this);
   }

   String mName;
   char*  mpCode;
   int    mInstruction;
   int    mArguments;
   int    mInterface;
};

#endif // VIRTUAL_FUNCTION_TABLE_ENTRY_H_
