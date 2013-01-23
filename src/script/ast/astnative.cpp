
#include "astnative.h"

#include "astvisitor.h"

ASTNative::ASTNative():
   ASTExpressionPart()
{
}

// - Get/set

bool ASTNative::hasArguments() const
{
   return !getChildren().isEmpty();
}

const ASTNodes& ASTNative::getArguments() const
{
   return getChildren();
}
   
ASTNodes& ASTNative::getArguments()
{
   return getChildren();
}

int ASTNative::getIndex() const
{
   return mIndex;
}

void ASTNative::setIndex(int index)
{
   mIndex = index;
}

// - Visitor

ACCEPT_IMPL(ASTNative);