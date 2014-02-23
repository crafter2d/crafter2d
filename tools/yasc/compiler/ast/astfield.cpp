
#include "astfield.h"

#include "astclass.h"
#include "astexpression.h"
#include "astvariable.h"
#include "astvisitor.h"

ASTField::ASTField(ASTVariable* pvariable):
   ASTMember(ASTMember::eField),
   mpVariable(pvariable)
{
}

ASTField::~ASTField()
{
   setVariable(NULL);
}

// - Get/set

const ASTVariable& ASTField::getVariable() const
{
   return *mpVariable;
}
         
ASTVariable& ASTField::getVariable()
{
   return *mpVariable;
}

void ASTField::setVariable(ASTVariable* pvariable)
{
   delete mpVariable;
   mpVariable = pvariable;
}

// - Query

String ASTField::getQualifiedName() const
{
   return getClass().getFullName() + L'.' + mpVariable->getName();
}

// - Visitor

ACCEPT_IMPL(ASTField)
