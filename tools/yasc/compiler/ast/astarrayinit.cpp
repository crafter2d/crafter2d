
#include "astarrayinit.h"

#include "astexpression.h"
#include "astvariableinit.h"
#include "astvisitor.h"

ASTArrayInit::ASTArrayInit():
   ASTNode(),
   mpType(nullptr)
{
}

// - Get/set

const ASTType& ASTArrayInit::getType() const
{
   ASSERT_PTR(mpType);
   return *mpType;
}

int ASTArrayInit::getCount() const
{
   return getChildren().size();
}

const ASTVariableInit& ASTArrayInit::getInit(int index) const
{
   return static_cast<const ASTVariableInit&>(getChildren()[index]);
}

// - Operations

// for now a simple array detection: 1 level
void ASTArrayInit::determineType()
{
   const ASTNodes& entries = getChildren();
   const ASTVariableInit& varinit = static_cast<const ASTVariableInit&>(entries[0]);   
   ASSERT(varinit.hasExpression());
}

// - Visitor

ACCEPT_IMPL(ASTArrayInit);
