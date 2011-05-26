
#include "astfunctionargument.h"

#include "astvariable.h"
#include "astvisitor.h"

ASTFunctionArgument::ASTFunctionArgument(ASTVariable* pvariable):
   ASTNode(),
   mpVariable(pvariable)
{
}

ASTFunctionArgument::~ASTFunctionArgument()
{
   setVariable(NULL);
}

// - Get/set

const ASTVariable& ASTFunctionArgument::getVariable() const
{
   return *mpVariable;
}
         
ASTVariable& ASTFunctionArgument::getVariable()
{
   return *mpVariable;
}

void ASTFunctionArgument::setVariable(ASTVariable* pvariable)
{
   delete mpVariable;
   mpVariable = pvariable;
}

// - Visitor

ACCEPT_IMPL(ASTFunctionArgument)