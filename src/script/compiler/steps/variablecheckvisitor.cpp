
#include "variablecheckvisitor.h"

#include "ast/ast.h"
#include "ast/astvisitor.h"

// static 
bool VariableCheckVisitor::isVariable(const ASTNode& node)
{
   VariableCheckVisitor visitor;
   return visitor.isVariableImpl(node);
}

VariableCheckVisitor::VariableCheckVisitor():
   ASTVisitor(),
   mVariable(NULL)
{
}

// - Query

bool VariableCheckVisitor::isVariableImpl(const ASTNode& node)
{
   node.accept(*this);
   return mVariable;
}

// - Visitor

void VariableCheckVisitor::visit(const ASTExpression& ast)
{
}

void VariableCheckVisitor::visit(const ASTUnary& ast)
{
   visitChildren(ast);
}

void VariableCheckVisitor::visit(const ASTCast& ast)
{
   ast.getNode().accept(*this);
}

void VariableCheckVisitor::visit(const ASTAccess& ast)
{
   switch ( ast.getKind() )
   {
      case ASTAccess::eVariable:
         mVariable = true;
         break;

      case ASTAccess::eFunction:
         mVariable = false;
         break;

      case ASTAccess::eArray:
         mVariable = true;
         break;
   }
}
