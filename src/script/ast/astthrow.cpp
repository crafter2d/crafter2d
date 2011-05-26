
#include "astthrow.h"

#include "astexpression.h"
#include "astvisitor.h"

ASTThrow::ASTThrow():
   ASTStatement(),
   mpExpression(NULL)
{
}

ASTThrow::~ASTThrow()
{
   setExpression(NULL);
}

// - Get/set

const ASTExpression& ASTThrow::getExpression() const
{
   return *mpExpression;
}

ASTExpression& ASTThrow::getExpression()
{
   return *mpExpression;
}

void ASTThrow::setExpression(ASTExpression* pexpression)
{
   delete mpExpression;
   mpExpression = pexpression;
}

// - Visitor

ACCEPT_IMPL(ASTThrow)
