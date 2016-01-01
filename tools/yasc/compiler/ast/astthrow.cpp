
#include "astthrow.h"

#include "core/defines.h"

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

// - Query

bool ASTThrow::hasReturn(bool& hasunreachablecode) const
{
   C2D_UNUSED(hasunreachablecode);
   return true;
}

// - Visitor

ACCEPT_IMPL(ASTThrow)
