
#include "scopevariable.h"

#include "yasc/compiler/ast/astvariable.h"
#include "yasc/compiler/ast/asttype.h"

// static 
ScopeVariable* ScopeVariable::fromVariable(const ASTVariable& variable)
{
   ScopeVariable* pvariable = new ScopeVariable(const_cast<ASTVariable&>(variable));
   return pvariable;
}

ScopeVariable::ScopeVariable(ASTVariable& variable):
   mVariable(variable)
{
}

ScopeVariable::~ScopeVariable()
{
}

// - Get/set

const String& ScopeVariable::getName() const
{
   return mVariable.getName();
}

const ASTType& ScopeVariable::getType() const
{
   return mVariable.getType();
}

int ScopeVariable::getResourceIndex() const
{
   return mVariable.getResourceIndex();
}

const ASTVariable& ScopeVariable::getVariable() const
{
   return mVariable;
}

ASTVariable& ScopeVariable::getVariable()
{
   return mVariable;
}

