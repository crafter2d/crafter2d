
#include "virtualfunctiontableentry.h"

#include "virtuallookuptable.h"

VirtualFunctionTableEntry::VirtualFunctionTableEntry():
   mName(),
   guards(),
   mInstruction(-1),
   mArguments(0),
   mLocals(0),
   mInterface(-1),
   returns(false)
{
}

VirtualFunctionTableEntry::VirtualFunctionTableEntry(const VirtualFunctionTableEntry& that):
   mName(that.mName),
   guards(that.guards),
   mInstruction(that.mInstruction),
   mArguments(that.mArguments),
   mLocals(that.mLocals),
   mInterface(that.mInterface),
   returns(that.returns)
{
}

VirtualFunctionTableEntry* VirtualFunctionTableEntry::clone() const
{
   return new VirtualFunctionTableEntry(*this);
}

void VirtualFunctionTableEntry::addLookupTable(VirtualLookupTable* ptable)
{
   lookups.push_back(ptable);
}

void VirtualFunctionTableEntry::addGuard(VirtualGuard* pguard)
{
   guards.add(pguard);
}

void VirtualFunctionTableEntry::updateGuards()
{
   guards.updatePosition(mInstruction);
}

void VirtualFunctionTableEntry::updateLookupTables()
{
   for ( std::size_t index = 0; index < lookups.size(); ++index )
   {
      lookups[index]->updatePosition(mInstruction);
   }
}