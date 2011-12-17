
#include "preloadvisitor.h"

#include "script/compiler/compiler.h"
#include "script/compiler/exceptions/classnotfoundexception.h"

#include "script/ast/ast.h"
#include "script/scope/scopedscope.h"
#include "script/scope/scopevariable.h"

PreloadVisitor::PreloadVisitor(CompileContext& context):
   CompileStep(),
   mContext(context),
   mClassResolver(),
   mPackage(),
   mScopeStack()
{
}

PreloadVisitor::~PreloadVisitor()
{
}

// - Overloads

bool PreloadVisitor::performStep(ASTNode& node)
{
   node.accept(*this);

   return true;
}

// - Visitor

void PreloadVisitor::visit(ASTRoot& ast)
{
   visitChildren(ast);
}

void PreloadVisitor::visit(ASTPackage& ast)
{
   mPackage = ast.getName();

   mClassResolver.insert(mPackage + ".*");
}

void PreloadVisitor::visit(ASTUse& ast)
{
   mClassResolver.insert(ast.getIdentifier());
}

void PreloadVisitor::visit(ASTClass& ast)
{
   ScopedScope scope(mScopeStack);

   mContext.addClass(&ast);

   if ( ast.hasBaseType() && !load(ast.getBaseType()) )
   {
      return;
   }

   ASTTypeList& intrfaces = ast.getInterfaces();
   for ( int index = 0; index < intrfaces.size(); index++ )
   {
      ASTType& type = intrfaces[index];
      if ( !load(type) )
      {
         // could not load type
      }
   }
   
   visitChildren(ast);

   ast.setState(ASTClass::eLoaded);
}

void PreloadVisitor::visit(ASTFunction& ast)
{
   ScopedScope scope(mScopeStack);

   if ( !load(ast.getType()) )
   {
      // type is not known :(
   }

   visitChildren(ast); // <-- arguments

   if ( ast.hasBody() )
   {
      ast.getBody().accept(*this);
   }
}

void PreloadVisitor::visit(ASTFunctionArgument& ast)
{
   ASTVariable& var = ast.getVariable();

   if ( !load(var.getType()) ) // <-- need to give class as param so the previous typevariable stuff can be moved there
   {
      // complain!
   }

   if ( var.hasExpression() )
   {
      var.getExpression().accept(*this);
   }

   ScopeVariable* pvariable = ScopeVariable::fromVariable(var);
   mScopeStack.add(pvariable);
}

void PreloadVisitor::visit(ASTField& ast)
{
   ASTVariable& var = ast.getVariable();

   if ( !load(var.getType()) )
   {
      // complain!
   }

   if ( var.hasExpression() )
   {
      var.getExpression().accept(*this);
   }

   ScopeVariable* pvariable = ScopeVariable::fromVariable(var);
   mScopeStack.add(pvariable);
}

void PreloadVisitor::visit(ASTBlock& ast)
{
   ScopedScope scope(mScopeStack);

   visitChildren(ast);
}

void PreloadVisitor::visit(ASTLocalVariable& ast)
{
   ASTVariable& var = ast.getVariable();

   if ( !load(var.getType()) )
   {
      // complain
   }

   if ( var.hasExpression() )
   {
      var.getExpression().accept(*this);
   }

   ScopeVariable* pvariable = ScopeVariable::fromVariable(var);
   mScopeStack.add(pvariable);
}

void PreloadVisitor::visit(ASTExpressionStatement& ast)
{
   if ( ast.hasExpression() )
   {
      ast.getExpression().accept(*this);
   }
}

void PreloadVisitor::visit(ASTIf& ast)
{
   ast.getCondition().accept(*this);
   ast.getStatement().accept(*this);

   if ( ast.hasElseStatement() )
   {
      ast.getElseStatement().accept(*this);
   }
}

void PreloadVisitor::visit(ASTFor& ast)
{
   ScopedScope scope(mScopeStack);

   if ( ast.hasInitializer() )
   {
      ast.getInitializer().accept(*this);
   }

   if ( ast.hasCondition() )
   {
      ast.getCondition().accept(*this);
   }

   ast.getBody().accept(*this);

   visitChildren(ast); // <-- loop expressions
}

void PreloadVisitor::visit(ASTForeach& ast)
{
   ASTVariable& var = ast.getVariable();

   if ( !load(var.getType()) )
   {
      // complain
   }

   if ( var.hasExpression() )
   {
      var.getExpression().accept(*this);
   }

   ScopeVariable* pvariable = ScopeVariable::fromVariable(var);
   mScopeStack.add(pvariable);

   ast.getBody().accept(*this);
}

void PreloadVisitor::visit(ASTWhile& ast)
{
   ScopedScope scope(mScopeStack);

   ast.getCondition().accept(*this);
   ast.getBody().accept(*this);
}

void PreloadVisitor::visit(ASTDo& ast)
{
   ScopedScope scope(mScopeStack);

   ast.getBody().accept(*this);
   ast.getCondition().accept(*this);
}

void PreloadVisitor::visit(ASTSwitch& ast)
{
   ast.getExpression().accept(*this);

   visitChildren(ast);
}

void PreloadVisitor::visit(ASTCase& ast)
{
   ast.getBody().accept(*this);
}

void PreloadVisitor::visit(ASTReturn& ast)
{
   if ( ast.hasExpression() )
   {
      ast.getExpression().accept(*this);
   }
}

void PreloadVisitor::visit(ASTTry& ast)
{
   ast.getBody().accept(*this);

   if ( ast.hasFinallyBlock() )
   {
      ast.getFinallyBlock().accept(*this);
   }

   visitChildren(ast);
}

void PreloadVisitor::visit(ASTCatch& ast)
{
   ast.getVariable().accept(*this);
   ast.getBody().accept(*this);
}

void PreloadVisitor::visit(ASTThrow& ast)
{
   ast.getExpression().accept(*this);
}

void PreloadVisitor::visit(ASTExpression& ast)
{
   ast.getLeft().accept(*this);

   if ( ast.hasRight() )
   {
      ast.getRight().accept(*this);
   }
}

void PreloadVisitor::visit(ASTConcatenate& ast)
{
   ast.getLeft().accept(*this);
   ast.getRight().accept(*this);
}

void PreloadVisitor::visit(ASTUnary& ast)
{
   visitChildren(ast);
}

void PreloadVisitor::visit(ASTNew& ast)
{
   if ( !load(ast.getType()) )
   {
      // complain!
   }

   visitChildren(ast);
}

void PreloadVisitor::visit(ASTCast& ast)
{
   if ( !load(ast.getType()) )
   {
      // complain!
   }

   ast.getNode().accept(*this);
}

void PreloadVisitor::visit(ASTAccess& ast)
{
   switch ( ast.getKind() )
   {
      case ASTAccess::eVariable:
         {
            ScopeVariable* pvariable = mScopeStack.find(ast.getName());
            if ( pvariable == NULL )
            {
               // not a variable, so see if it is a static class
               ASTType type(ASTType::eObject);
               type.setObjectName(ast.getName());
               
               tryLoad(type);
            }
         }
         break;

      case ASTAccess::eFunction:
         {
            ASTNodes& args = ast.getArguments();
            args.accept(*this);
         }
         break;

      case ASTAccess::eArray:
         {
            visitChildren(ast);
         }
         break;
   }
}

void PreloadVisitor::visit(ASTCompound& compound)
{
   compound.getExpression().accept(*this);
}

// -  Operations

bool PreloadVisitor::load(ASTType& type)
{
   if ( type.isArray() )
   {
      return load(type.getArrayType());
   }
   else if ( type.isObject() )
   {
      const std::string& name = type.getObjectName();

      /*
      // check the type arguments
      const ASTTypeList& typearguments = type.getTypeArguments();
      for ( int index = 0; index < typearguments.size(); index++ )
      {
         if ( !mTypeArguments[index].resolveType(context, aclass) )
         {
            return false;
         }
      }
      */
      if ( type.getObjectName() == "T" )
      {
         return true;
      }

      if ( !tryLoad(type) )
      {
         throw new ClassNotFoundException(type.getObjectName());
      }
   }

   return true;
}

bool PreloadVisitor::tryLoad(ASTType& type)
{
   return mContext.loadClass(type.getObjectName());
}
