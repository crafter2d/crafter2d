
#include "symbolcollectorstep.h"

#include "script/compiler/compilecontext.h"

#include "script/ast/ast.h"

SymbolCollectorVisitor::SymbolCollectorVisitor(CompileContext& context):
   CompileStep(),
   mContext(context),
   mPackage(),
   mResolver(),
   mpClass(NULL)
{
}

SymbolCollectorVisitor::~SymbolCollectorVisitor()
{
}

// - Overloads

bool SymbolCollectorVisitor::performStep(ASTNode& node)
{
   ASTClass* pcurrent = mpClass;

   node.accept(*this);

   mpClass = pcurrent;

   return true;
}

// - Visitor

void SymbolCollectorVisitor::visit(ASTRoot& ast)
{
   visitChildren(ast);
}

void SymbolCollectorVisitor::visit(ASTPackage& ast)
{
   mPackage = ast.getName();
}

void SymbolCollectorVisitor::visit(ASTUse& ast)
{
   mContext.addPath(ast.getIdentifier());
}

void SymbolCollectorVisitor::visit(ASTClass& ast)
{
   mpClass = &ast;
   mContext.addClass(&ast);

   if ( ast.getName().compare("Object") != 0 )
   {
      if ( !ast.hasBaseType() )
      {
         ASTType* ptype = new ASTType(ASTType::eObject);
         ptype->setObjectName("Object");
         ast.setBaseType(ptype);
      }

      if ( !ast.getBaseType().resolveType(mContext, ast) )
      {
         // meh, class unknown!
      }
   }

   ASTTypeList& intrfaces = ast.getInterfaces();
   for ( int index = 0; index < intrfaces.size(); index++ )
   {
      ASTType& type = intrfaces[index];
      if ( !type.resolveType(mContext, ast) )
      {
         mContext.getLog().error("Can not resolve type " + type.toString());
      }
   }

   if ( !ast.hasConstructor() )
   {
      createDefaultConstructor(ast);
   }

   visitChildren(ast);

   mpClass->calculateResources();
}

void SymbolCollectorVisitor::visit(ASTFunction& ast)
{
   if ( !ast.getType().resolveType(mContext, *mpClass) )
   {
      // type is not known :(
   }

   visitChildren(ast); // <-- arguments

   if ( ast.hasBody() )
   {
      ast.getBody().accept(*this);
   }
   else if ( !ast.getModifiers().isAbstract() && !ast.getModifiers().isNative() )
   {
      mContext.getLog().error("expecting a body for function " + mpClass->getName() + "." + ast.getName());
   }
}

void SymbolCollectorVisitor::visit(ASTFunctionArgument& ast)
{
   ASTVariable& var = ast.getVariable();

   if ( !var.getType().resolveType(mContext, *mpClass) ) // <-- need to give class as param so the previous typevariable stuff can be moved there
   {
      // complain!
   }

   if ( var.hasExpression() )
   {
      var.getExpression().accept(*this);
   }
}

void SymbolCollectorVisitor::visit(ASTField& ast)
{
   ASTVariable& var = ast.getVariable();

   if ( !var.getType().resolveType(mContext, *mpClass) )
   {
      // complain!
   }

   if ( var.hasExpression() )
   {
      var.getExpression().accept(*this);
   }
}

void SymbolCollectorVisitor::visit(ASTBlock& ast)
{
   visitChildren(ast);
}

void SymbolCollectorVisitor::visit(ASTLocalVariable& ast)
{
   ASTVariable& var = ast.getVariable();

   if ( !var.getType().resolveType(mContext, *mpClass) )
   {
      // complain
   }

   if ( var.hasExpression() )
   {
      var.getExpression().accept(*this);
   }
}

void SymbolCollectorVisitor::visit(ASTExpressionStatement& ast)
{
   if ( ast.hasExpression() )
   {
      ast.getExpression().accept(*this);
   }
}

void SymbolCollectorVisitor::visit(ASTIf& ast)
{
   ast.getCondition().accept(*this);
   ast.getStatement().accept(*this);

   if ( ast.hasElseStatement() )
   {
      ast.getElseStatement().accept(*this);
   }
}

void SymbolCollectorVisitor::visit(ASTFor& ast)
{
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

void SymbolCollectorVisitor::visit(ASTForeach& ast)
{
   ASTVariable& var = ast.getVariable();

   if ( !var.getType().resolveType(mContext, *mpClass) )
   {
      // complain
   }

   if ( var.hasExpression() )
   {
      var.getExpression().accept(*this);
   }

   ast.getBody().accept(*this);
}

void SymbolCollectorVisitor::visit(ASTWhile& ast)
{
   ast.getCondition().accept(*this);
   ast.getBody().accept(*this);
}

void SymbolCollectorVisitor::visit(ASTDo& ast)
{
   ast.getBody().accept(*this);
   ast.getCondition().accept(*this);
}

void SymbolCollectorVisitor::visit(ASTReturn& ast)
{
   if ( ast.hasExpression() )
   {
      ast.getExpression().accept(*this);
   }
}

void SymbolCollectorVisitor::visit(ASTTry& ast)
{
   ast.getBody().accept(*this);

   if ( ast.hasFinallyBlock() )
   {
      ast.getFinallyBlock().accept(*this);
   }

   visitChildren(ast);
}

void SymbolCollectorVisitor::visit(ASTCatch& ast)
{
   ast.getVariable().accept(*this);
   ast.getBody().accept(*this);
}

void SymbolCollectorVisitor::visit(ASTThrow& ast)
{
   ast.getExpression().accept(*this);
}

void SymbolCollectorVisitor::visit(ASTExpression& ast)
{
   ast.getLeft().accept(*this);

   if ( ast.hasRight() )
   {
      ast.getRight().accept(*this);
   }
}

void SymbolCollectorVisitor::visit(ASTConcatenate& ast)
{
   ast.getLeft().accept(*this);
   ast.getRight().accept(*this);
}

void SymbolCollectorVisitor::visit(ASTUnary& ast)
{
   visitChildren(ast);
}

void SymbolCollectorVisitor::visit(ASTNew& ast)
{
   if ( !ast.getType().resolveType(mContext, *mpClass) )
   {
      // complain!
   }

   visitChildren(ast);
}

void SymbolCollectorVisitor::visit(ASTCast& ast)
{
   if ( !ast.getType().resolveType(mContext, *mpClass) )
   {
      // complain!
   }

   ast.getNode().accept(*this);
}

void SymbolCollectorVisitor::visit(ASTAccess& ast)
{
   switch ( ast.getKind() )
   {
      case ASTAccess::eVariable:
         {
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

void SymbolCollectorVisitor::visit(ASTCompound& compound)
{
   compound.getExpression().accept(*this);
}

// - Operations

void SymbolCollectorVisitor::createDefaultConstructor(ASTClass& ast)
{
   // default constructor always calls super class and is public

   ASTBlock* pbody = new ASTBlock();
   ASTSuper* psuper = new ASTSuper();
   psuper->setCall(true);
   psuper->setKind(ASTSuper::eSuper);
   pbody->addChild(psuper);
   
   ASTFunction* pconstructor = new ASTFunction(ASTMember::eConstructor);
   pconstructor->setName(ast.getName());
   pconstructor->setBody(pbody);
   pconstructor->setType(new ASTType(ASTType::eVoid));

   ast.addMember(pconstructor);
}
