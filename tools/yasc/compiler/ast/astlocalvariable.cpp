

#include "astlocalvariable.h"

#include "astvariable.h"
#include "astvisitor.h"

ASTLocalVariable::ASTLocalVariable(ASTVariable* pvariable):
   ASTStatement(),
   mpVariable(pvariable)
{
   setPosition(pvariable->getPosition());
}

ASTLocalVariable::~ASTLocalVariable()
{
   setVariable(nullptr);
}

// - Get/set

const ASTVariable& ASTLocalVariable::getVariable() const
{
   return *mpVariable;
}
         
ASTVariable& ASTLocalVariable::getVariable()
{
   return *mpVariable;
}

void ASTLocalVariable::setVariable(ASTVariable* pvariable)
{
   delete mpVariable;
   mpVariable = pvariable;
}

// - Visitor

ACCEPT_IMPL(ASTLocalVariable)
