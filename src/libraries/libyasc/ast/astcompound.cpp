
#include "astcompound.h"

#include "astexpression.h"
#include "astvisitor.h"

ASTCompound::ASTCompound():
   ASTExpressionPart(),
   mpExpression(nullptr)
{
}

ASTCompound::~ASTCompound()
{
   setExpression(nullptr);
}

// - Get/set

const ASTExpression& ASTCompound::getExpression() const
{
   return *mpExpression;
}

ASTExpression& ASTCompound::getExpression()
{
   return *mpExpression;
}

void ASTCompound::setExpression(ASTExpression* pexpression)
{
   delete mpExpression;
   mpExpression = pexpression;
}

// - Visitor

ACCEPT_IMPL(ASTCompound)