
#include "astreturn.h"

#include "core/defines.h"

#include "astexpression.h"
#include "astvisitor.h"

ASTReturn::ASTReturn():
   ASTStatement(),
   mpExpression(nullptr)
{
}

ASTReturn::~ASTReturn()
{
   setExpression(nullptr);
}

// - Get/set

bool ASTReturn::hasExpression() const
{
   return mpExpression != nullptr;
}

const ASTExpression& ASTReturn::getExpression() const
{
   return *mpExpression;
}

ASTExpression& ASTReturn::getExpression()
{
   return *mpExpression;
}

void ASTReturn::setExpression(ASTExpression* pexpression)
{
   delete mpExpression;
   mpExpression = pexpression;
}

// - Query

bool ASTReturn::hasReturn(bool& hasunreachablecode) const
{
   C2D_UNUSED(hasunreachablecode);
   return true;
}

// - Visitor
   
ACCEPT_IMPL(ASTReturn)
