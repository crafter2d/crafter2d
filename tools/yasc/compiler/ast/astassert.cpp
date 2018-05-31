
#include "astassert.h"

#include "core/defines.h"

#include "astexpression.h"
#include "astvisitor.h"

ASTAssert::ASTAssert():
   ASTStatement(),
   mpCondition(nullptr)
{
}

ASTAssert::~ASTAssert()
{
   setCondition(nullptr);
}

// - Get/set
   
const ASTExpression& ASTAssert::getCondition() const
{
   ASSERT_PTR(mpCondition);
   return *mpCondition;
}

ASTExpression& ASTAssert::getCondition()
{
   ASSERT_PTR(mpCondition);
   return *mpCondition;
}

void ASTAssert::setCondition(ASTExpression* pcondition)
{
   delete mpCondition;
   mpCondition = pcondition;
}

// - Query
   
bool ASTAssert::hasReturn(bool& hasunreachablecode) const
{
   C2D_UNUSED(hasunreachablecode);
   
   return false;
}

// - Visitor

ACCEPT_IMPL(ASTAssert)
