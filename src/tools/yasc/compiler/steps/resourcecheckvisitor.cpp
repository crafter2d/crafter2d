
#include "resourcecheckvisitor.h"

#include "yasc/compiler/ast/ast.h"
#include "yasc/compiler/compilecontext.h"

ResourceCheckVisitor::ResourceCheckVisitor(CompileContext& context):
   CompileStep(context),
   mOffset(0)
{
}

// - Overloads

bool ResourceCheckVisitor::performStep(ASTNode& node)
{
   node.accept(*this);
   return true;
}

// - Visitor

void ResourceCheckVisitor::visit(ASTRoot& ast)
{
   visitChildren(ast);
}

void ResourceCheckVisitor::visit(ASTClass& ast)
{
   visitChildren(ast);
}

void ResourceCheckVisitor::visit(ASTFunction& ast)
{
   if ( !ast.getModifiers().isPureNative() && !ast.getModifiers().isAbstract() )
   {
      mOffset = 0;

      ast.getBody().accept(*this);
    
      ast.setLocalVariableCount(mOffset - ast.getArguments().size());
   }
}

void ResourceCheckVisitor::visit(ASTBlock& ast)
{
   visitChildren(ast);
}

void ResourceCheckVisitor::visit(ASTLocalVariable& ast)
{
   ast.getVariable().setResourceIndex(mOffset);

   mOffset++;
}

void ResourceCheckVisitor::visit(ASTIf& ast)
{
   ast.getStatement().accept(*this);
   if ( ast.hasElseStatement() )
   {
      ast.getElseStatement().accept(*this);
   }
}

void ResourceCheckVisitor::visit(ASTFor& ast)
{
   if ( ast.hasInitializer() )
   {
      ast.getInitializer().accept(*this);
   }

   ast.getBody().accept(*this);
}

void ResourceCheckVisitor::visit(ASTForeach& ast)
{
   ast.getIteratorVariable().setResourceIndex(mOffset++);
   ast.getVariable().setResourceIndex(mOffset++);   
   ast.getBody().accept(*this);
}

void ResourceCheckVisitor::visit(ASTWhile& ast)
{
   // condition can not have a local variable
   ast.getBody().accept(*this);
}

void ResourceCheckVisitor::visit(ASTDo& ast)
{
   ast.getBody().accept(*this);
   // condition can not have a local variable
}

void ResourceCheckVisitor::visit(ASTSwitch& ast)
{
   ast.getExpression().accept(*this);

   visitChildren(ast);
}
   
void ResourceCheckVisitor::visit(ASTCase& ast)
{
   ast.getBody().accept(*this);
}

void ResourceCheckVisitor::visit(ASTTry& ast)
{
   ast.getBody().accept(*this);

   if ( ast.getCatches().size() > 0 )
   {
      ast.setResourceIndex(mOffset++);

      visitChildren(ast);
   }

   if ( ast.hasFinallyBlock() )
   {
      ast.getFinallyBlock().accept(*this);
   }
}

void ResourceCheckVisitor::visit(ASTCatch& ast)
{
   // the variable of the catches are shared   
   ast.getBody().accept(*this);
}
