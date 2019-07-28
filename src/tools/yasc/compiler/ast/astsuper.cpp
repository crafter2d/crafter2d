
#include "astsuper.h"

#include "astvisitor.h"

ASTSuper::ASTSuper():
   ASTExpressionPart(),
   mpConstructor(nullptr),
   mKind(eThis),
   mCall(false)
{
}

// - Get/set
  
bool ASTSuper::hasConstructor() const
{
   return mpConstructor != nullptr;
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

int ASTSuper::getArgumentCount() const
{
   return getChildren().size();
}

ASTNodes& ASTSuper::getArguments()
{
   return getChildren();
}

bool ASTSuper::isSuper() const
{
   return mKind == eSuper;
}

bool ASTSuper::isThis() const
{
   return mKind == eThis;
}

// - Visitor

ACCEPT_IMPL(ASTSuper)
