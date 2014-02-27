
#include "virtualfunctiontableentry.h"

#include "core/defines.h"

#include "mod_yas/common/type.h"

#include "virtualfunction.h"
#include "virtuallookuptable.h"

// - Statics

VirtualFunctionTableEntry* VirtualFunctionTableEntry::fromFunction(VirtualFunction& function)
{
   VirtualFunctionTableEntry* presult = new VirtualFunctionTableEntry();
   presult->setFunction(function);
   return presult;
}


// - Implementation

VirtualFunctionTableEntry::VirtualFunctionTableEntry():
   mpFunction(NULL),
   mArguments(0),
   mLocals(0),
   returns(false)
{
}

VirtualFunctionTableEntry::VirtualFunctionTableEntry(const VirtualFunctionTableEntry& that):
   mpFunction(that.mpFunction),
   mArguments(that.mArguments),
   mLocals(that.mLocals),
   returns(that.returns)
{
}

// - Operations

const String& VirtualFunctionTableEntry::getName() const
{
   return mpFunction->getName();
}

int VirtualFunctionTableEntry::lookup(int tableid, const VirtualValue& value) const
{
   return mpFunction->lookup(tableid, value);
}

const VirtualGuard* VirtualFunctionTableEntry::findGuard(int ip) const
{
   return mpFunction->findGuard(ip);
}

void VirtualFunctionTableEntry::setFunction(VirtualFunction& function)
{
   mpFunction = &function;
   mArguments = function.getArguments().size() + (function.getModifiers().isStatic() ? 0 : 1);
   mLocals = function.getLocals().size();
   returns = !function.getReturnType().isVoid();
}

// - Maintenance

VirtualFunctionTableEntry* VirtualFunctionTableEntry::clone() const
{
   return new VirtualFunctionTableEntry(*this);
}
