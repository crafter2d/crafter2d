
#include "astreturn.h"

#include "astexpression.h"
#include "astvisitor.h"

ASTReturn::ASTReturn():
   ASTStatement(),
   mpExpression(NULL)
{
}

ASTReturn::~ASTReturn()
{
   setExpression(NULL);
}

// - Get/set

bool ASTReturn::hasExpression() const
{
   return mpExpression != NULL;
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
   return true;
}

// - Visitor
   
ACCEPT_IMPL(ASTReturn)
