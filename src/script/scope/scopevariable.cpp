
#include "scopevariable.h"

#include "script/ast/astvariable.h"
#include "script/ast/asttype.h"

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

const std::string& ScopeVariable::getName() const
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
