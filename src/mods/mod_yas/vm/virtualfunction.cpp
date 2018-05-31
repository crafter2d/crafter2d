
#include "virtualfunction.h"

#include "core/defines.h"

#include "mod_yas/common/type.h"

#include "virtuallookuptable.h"

VirtualFunction::VirtualFunction():
   mName(),
   mpClass(nullptr),
   mModifiers(),
   mArguments(),
   mLocals(),
   mReturnType(),
   mGuards(),
   mLookups(),
   mInstructions(),
   mpCode(nullptr),
   mCodeLength(-1),
   mIndex(-1)
{
}

VirtualFunction::~VirtualFunction()
{
   if ( mpCode != nullptr )
   {
      free(mpCode);
      mpCode = nullptr;
   }
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
   return mReturnType;
}

void VirtualFunction::setReturnType(const yasc::Type& type)
{
   mReturnType = type;
}

const StringList& VirtualFunction::getAnnotations() const
{
   return mAnnotations;
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

bool VirtualFunction::hasCode() const
{
   return mpCode != nullptr;
}

const char* VirtualFunction::getCode()
{
   return mpCode;
}

void VirtualFunction::setCode(char* pcode, int len)
{
   mpCode = pcode;
   mCodeLength = len;
}

int VirtualFunction::getIndex() const
{
   return mIndex;
}

void VirtualFunction::setIndex(int index)
{
   mIndex = index;
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

int VirtualFunction::getArgumentCount() const
{
   return mArguments.size() + (mModifiers.isStatic() ? 0 : 1);
}

// - Operations

void VirtualFunction::addAnnotation(const String& annotation)
{
   mAnnotations.add(annotation);
}

void VirtualFunction::addLookupTable(VirtualLookupTable* ptable)
{
   mLookups.add(ptable);
}

void VirtualFunction::addGuard(const VirtualGuard& guard)
{
   mGuards.add(guard);
}

// - Search

const VirtualGuard* VirtualFunction::findGuard(int ip) const
{
   return mGuards.findGuard(ip);
}
