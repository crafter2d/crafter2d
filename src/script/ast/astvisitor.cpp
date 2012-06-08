
#include "astvisitor.h"

#include "astnode.h"
#include "astnodes.h"

ASTVisitor::ASTVisitor()
{
}

ASTVisitor::~ASTVisitor()
{
}

// - Acceptors

VISIT_IMPL(ASTRoot)
VISIT_IMPL(ASTPackage)
VISIT_IMPL(ASTUse)
VISIT_IMPL(ASTClass)
VISIT_IMPL(ASTField)
VISIT_IMPL(ASTFunction)
VISIT_IMPL(ASTFunctionArgument)
VISIT_IMPL(ASTLocalVariable)
VISIT_IMPL(ASTAnnotations)
VISIT_IMPL(ASTArrayInit)
VISIT_IMPL(ASTBlock)
VISIT_IMPL(ASTIf)
VISIT_IMPL(ASTFor)
VISIT_IMPL(ASTForeach)
VISIT_IMPL(ASTWhile)
VISIT_IMPL(ASTDo)
VISIT_IMPL(ASTSwitch)
VISIT_IMPL(ASTCase)
VISIT_IMPL(ASTReturn)
VISIT_IMPL(ASTTry)
VISIT_IMPL(ASTCatch)
VISIT_IMPL(ASTThrow)
VISIT_IMPL(ASTAssert)
VISIT_IMPL(ASTLoopControl)
VISIT_IMPL(ASTLiteral)
VISIT_IMPL(ASTExpressionStatement)
VISIT_IMPL(ASTExpression)
VISIT_IMPL(ASTCast)
VISIT_IMPL(ASTUnary)
VISIT_IMPL(ASTInstanceOf)
VISIT_IMPL(ASTNew)
VISIT_IMPL(ASTSuper)
VISIT_IMPL(ASTAccess)
VISIT_IMPL(ASTCompound)
VISIT_IMPL(ASTConcatenate)

// - Iteration

void ASTVisitor::visitChildren(const ASTNode& node)
{
   const ASTNodes& children = node.getChildren();
   for ( int index = 0; index < children.size(); index++ )
   {
      const ASTNode& child = children[index];
      child.accept(*this);
   }
}

void ASTVisitor::visitChildren(ASTNode& node)
{
   ASTNodes& children = node.getChildren();
   children.accept(*this);
}

void ASTVisitor::reverseVisitChildren(const ASTNode& node)
{
   const ASTNodes& children = node.getChildren();
   children.accept(*this);
}
