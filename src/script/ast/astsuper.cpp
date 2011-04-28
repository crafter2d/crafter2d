
#include "astsuper.h"

#include "astvisitor.h"

ASTSuper::ASTSuper():
   ASTExpressionPart(),
   mpConstructor(NULL),
   mKind(eThis),
   mCall(false)
{
}

// - Get/set
  
bool ASTSuper::hasConstructor() const
{
   return mpConstructor != NULL;
}

const ASTFunction& ASTSuper::getConstructor() const
{
   return *mpConstructor;
}
   
void ASTSuper::setConstructor(ASTFunction* pmember)
{
   mpConstructor = pmember;
}

ASTSuper::Kind ASTSuper::getKind() const
{
   return mKind;
}

void ASTSuper::setKind(Kind kind)
{
   mKind = kind;
}

bool ASTSuper::isCall() const
{
   return mCall;
}

void ASTSuper::setCall(bool call)
{
   mCall = call;
}

// - Query

ASTNodes& ASTSuper::getArguments()
{
   return getChildren();
}

// - Visitor

ACCEPT_IMPL(ASTSuper)
