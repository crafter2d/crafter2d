
#include "astexpressionstatement.h"

#include "astexpression.h"
#include "astvisitor.h"

ASTExpressionStatement::ASTExpressionStatement(ASTExpression* pexpression):
   ASTStatement(),
   mpExpression(pexpression)
{
}

ASTExpressionStatement::~ASTExpressionStatement()
{
   setExpression(NULL);
}

// - Get/set

bool ASTExpressionStatement::hasExpression() const
{
   return mpExpression != NULL;
}

const ASTExpression& ASTExpressionStatement::getExpression() const
{
   return *mpExpression;
}

ASTExpression& ASTExpressionStatement::getExpression()
{
   return *mpExpression;
}

void ASTExpressionStatement::setExpression(ASTExpression* pexpression)
{
   delete mpExpression;
   mpExpression = pexpression;
}

// - Visitor

ACCEPT_IMPL(ASTExpressionStatement)
