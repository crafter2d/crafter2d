
#include "virtualfunction.h"

#include "core/defines.h"

#include "mod_yas/common/type.h"

#include "virtuallookuptable.h"

VirtualFunction::VirtualFunction():
   mName(),
   mpClass(NULL),
   mModifiers(),
   mArguments(),
   mLocals(),
   mpReturnType(NULL),
   mGuards(),
   mLookups(),
   mInstructions(),
   mIndex(-1),
   mFirstInstruction(-1)
{
}

// - Get/set

const String& VirtualFunction::getName() const
{
   return mName;
}
   
void VirtualFunction::setName(const String& name)
{
   mName = name;
}

const VirtualClass& VirtualFunction::getClass() const
{
   ASSERT_PTR(mpClass);
   return *mpClass;
}
   
void VirtualFunction::setClass(VirtualClass& klass)
{
   mpClass = &klass;
}

const Modifiers& VirtualFunction::getModifiers() const
{
   return mModifiers;
}

void VirtualFunction::setModifiers(const Modifiers& modifiers)
{
   mModifiers = modifiers;
}

const yasc::Types& VirtualFunction::getLocals() const
{
   return mLocals;
}

void VirtualFunction::setLocals(const yasc::Types& types)
{
   mLocals = types;
}

const yasc::Types& VirtualFunction::getArguments() const
{
   return mArguments;
}

void VirtualFunction::setArguments(const yasc::Types& args)
{
   mArguments = args;
}

const yasc::Type& VirtualFunction::getReturnType() const
{
   ASSERT_PTR(mpReturnType);
   return *mpReturnType;
}

void VirtualFunction::setReturnType(const yasc::Type& type)
{
   mpReturnType = type.clone();
}

const CIL::Instructions& VirtualFunction::getInstructions() const
{
   return mInstructions;
}
   
void VirtualFunction::setInstructions(const CIL::Instructions& insts)
{
   mInstructions = insts;
}

VirtualGuards& VirtualFunction::getGuards()
{
   return mGuards;
}

const VirtualLookupTables& VirtualFunction::getLookupTables() const
{
   return mLookups;
}

VirtualLookupTables& VirtualFunction::getLookupTables()
{
   return mLookups;
}

int VirtualFunction::getIndex() const
{
   return mIndex;
}

void VirtualFunction::setIndex(int index)
{
   mIndex = index;
}

int VirtualFunction::getFirstInstruction() const
{
   return mFirstInstruction;
}

void VirtualFunction::setFirstInstruction(int first)
{
   mFirstInstruction = first;
}

// - Query

bool VirtualFunction::equals(const VirtualFunction& that) const
{
   return mName == that.mName && mArguments.equals(that.mArguments);
}

int VirtualFunction::lookup(int tableid, const VirtualValue& value) const
{
   return mLookups[tableid].lookup(value);
}

// - Operations

void VirtualFunction::addLookupTable(VirtualLookupTable* ptable)
{
   mLookups.add(ptable);
}

void VirtualFunction::updateLookupTables()
{
   for ( std::size_t index = 0; index < mLookups.size(); ++index )
   {
      VirtualLookupTable& table = mLookups[index];
      table.updatePosition(mFirstInstruction);
   }
}

void VirtualFunction::addGuard(VirtualGuard* pguard)
{
   mGuards.add(pguard);
}

void VirtualFunction::updateGuards()
{
   mGuards.updatePosition(mFirstInstruction);
}

// - Search

const VirtualGuard* VirtualFunction::findGuard(int ip) const
{
   return mGuards.findGuard(ip);
}
