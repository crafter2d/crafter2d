
#ifndef VIRTUAL_FUNCTION_TABLE_ENTRY_H_
#define VIRTUAL_FUNCTION_TABLE_ENTRY_H_

#include "core/string/string.h"

#include "virtualguards.h"

class VirtualFunction;
class VirtualLookupTable;
class VirtualValue;

class VirtualFunctionTableEntry
{
public:
   static VirtualFunctionTableEntry* fromFunction(VirtualFunction& function);

   VirtualFunctionTableEntry();
   VirtualFunctionTableEntry(const VirtualFunctionTableEntry& that);

 // interface
   const String& getName() const;

   int lookup(int tableid, const VirtualValue& value) const;

   const VirtualGuard* findGuard(int ip) const;

   void setFunction(VirtualFunction& function);

   void update();
   
 // maintenance
   VirtualFunctionTableEntry* clone() const;

 // data
   VirtualFunction* mpFunction;
   int              mInstruction;
   int              mArguments;
   int              mLocals;
   bool             returns;
};

#endif // VIRTUAL_FUNCTION_TABLE_ENTRY_H_
