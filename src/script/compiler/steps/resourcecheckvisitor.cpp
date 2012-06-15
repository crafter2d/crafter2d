
#include "resourcecheckvisitor.h"

#include "script/ast/ast.h"
#include "script/compiler/compilecontext.h"

ResourceCheckVisitor::ResourceCheckVisitor(CompileContext& context):
   CompileStep(),
   mContext(context),
   mOffset(0),
   mMaxVariables(0)
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
   if ( !ast.getModifiers().isNative() && !ast.getModifiers().isAbstract() )
   {
      mOffset = ast.getArguments().size();
      mMaxVariables = mOffset;

      if ( !ast.getModifiers().isStatic() )
      {
         // add the this argument
         mOffset++;
      }
      
      ast.getBody().accept(*this);
    
      ast.setLocalVariableCount(mMaxVariables - ast.getArguments().size());
   }
}

void ResourceCheckVisitor::visit(ASTBlock& ast)
{
   int savedoffset = mOffset;

   visitChildren(ast);

   mMaxVariables = mOffset > mMaxVariables ? mOffset : mMaxVariables;
   mOffset = savedoffset;
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
   ast.getVariable().setResourceIndex(mOffset++);
   ast.setResourceIndex(mOffset++);
   
   ast.getBody().accept(*this);
}

void ResourceCheckVisitor::visit(ASTWhile& ast)
{
   ast.getCondition().accept(*this);
   ast.getBody().accept(*this);
}

void ResourceCheckVisitor::visit(ASTDo& ast)
{
   ast.getBody().accept(*this);
   ast.getCondition().accept(*this);
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
      ast.setResourceIndex(mOffset);
      mOffset++;

      visitChildren(ast);
   }

   if ( ast.hasFinallyBlock() )
   {
      ast.getFinallyBlock().accept(*this);
   }
}

void ResourceCheckVisitor::visit(ASTCatch& ast)
{
   ast.getBody().accept(*this);
}
