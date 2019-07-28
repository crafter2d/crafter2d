
#include "astforeach.h"

#include "astvariable.h"
#include "astvisitor.h"

ASTForeach::ASTForeach():
   ASTStatement(),
   mpVariable(nullptr),
   mpIterator(new ASTVariable()),
   mpBody(nullptr)
{
   createIteratorVariable();
}

ASTForeach::~ASTForeach()
{
   setVariable(nullptr);
   setIteratorVariable(nullptr);
   setBody(nullptr);
}

// - Get/set

const ASTVariable& ASTForeach::getVariable() const
{
   return *mpVariable;
}

ASTVariable& ASTForeach::getVariable()
{
   return *mpVariable;
}

void ASTForeach::setVariable(ASTVariable* pvariable)
{
   delete mpVariable;
   mpVariable = pvariable;
}

const ASTVariable& ASTForeach::getIteratorVariable() const
{
   return *mpIterator;
}
         
ASTVariable& ASTForeach::getIteratorVariable()
{
   return *mpIterator;
}

void ASTForeach::setIteratorVariable(ASTVariable* pvariable)
{
   delete mpIterator;
   mpIterator = pvariable;
}

const ASTStatement& ASTForeach::getBody() const
{
   return *mpBody;
}

ASTStatement& ASTForeach::getBody()
{
   return *mpBody;
}

void ASTForeach::setBody(ASTStatement* pbody)
{
   delete mpBody;
   mpBody = pbody;
}

// - Query
   
bool ASTForeach::hasReturn(bool& hasunreachablecode) const
{
   return getBody().hasReturn(hasunreachablecode);
}

// - Operations

void ASTForeach::createIteratorVariable()
{
   ASTModifiers modifiers;
   modifiers.setFlags(ASTModifiers::eInternal);

   mpIterator->setLocation(ASTVariable::eLocal);
   mpIterator->setName(UTEXT("it"));
   mpIterator->setModifiers(modifiers);
}

// - Visit

ACCEPT_IMPL(ASTForeach)
