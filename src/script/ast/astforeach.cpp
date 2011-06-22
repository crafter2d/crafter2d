
#include "astforeach.h"

#include "astvariable.h"
#include "astvisitor.h"

ASTForeach::ASTForeach():
   ASTStatement(),
   mpVariable(NULL),
   mpBody(NULL),
   mResourceIndex(-1)
{
}

ASTForeach::~ASTForeach()
{
   setVariable(NULL);
   setBody(NULL);
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

int ASTForeach::getResourceIndex() const
{
   return mResourceIndex;
}

void ASTForeach::setResourceIndex(int index)
{
   mResourceIndex = index;
}

// - Query
   
bool ASTForeach::hasReturn(bool& hasunreachablecode) const
{
   return getBody().hasReturn(hasunreachablecode);
}

// - Visit

ACCEPT_IMPL(ASTForeach)
