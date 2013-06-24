
#ifndef VIRTUAL_FUNCTION_TABLE_ENTRY_H_
#define VIRTUAL_FUNCTION_TABLE_ENTRY_H_

#include "core/string/string.h"

#include "virtualguards.h"

class VirtualLookupTable;

class VirtualFunctionTableEntry
{
public:
   typedef std::vector<VirtualLookupTable*> VirtualLookups;

   VirtualFunctionTableEntry();
   VirtualFunctionTableEntry(const VirtualFunctionTableEntry& that);

 // maintenance
   VirtualFunctionTableEntry* clone() const;

   void addLookupTable(VirtualLookupTable* ptable);
   void updateLookupTables();

   void addGuard(VirtualGuard* pguard);
   void updateGuards();

 // data
   String         mName;
   VirtualGuards  guards;
   VirtualLookups lookups;
   int            mInstruction;
   int            mArguments;
   int            mLocals;
   int            mInterface;
   bool           returns;
};

#endif // VIRTUAL_FUNCTION_TABLE_ENTRY_H_
