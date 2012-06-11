
#include "astarrayinit.h"

#include "astexpression.h"
#include "astvariableinit.h"
#include "astvisitor.h"

ASTArrayInit::ASTArrayInit():
   ASTNode(),
   mpType(NULL)
{
}

// - Get/set

const ASTType& ASTArrayInit::getType() const
{
   ASSERT_PTR(mpType);
   return *mpType;
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
