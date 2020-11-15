
#include "astloopcontrol.h"

#include "astvisitor.h"

ASTLoopControl::ASTLoopControl(Kind kind):
   ASTStatement(),
   mKind(kind)
{
}

// - Get/set
   
ASTLoopControl::Kind ASTLoopControl::getKind() const
{
   return mKind;
}

void ASTLoopControl::setKind(Kind kind)
{
   mKind = kind;
}

// - Visit

ACCEPT_IMPL(ASTLoopControl)
