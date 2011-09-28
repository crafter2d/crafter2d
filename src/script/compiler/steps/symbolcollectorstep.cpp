
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
   // add to the class, so it is local only and correct classes can be resolved
}

void SymbolCollectorVisitor::visit(ASTClass& ast)
{
   mpClass = &ast;

   if ( ast.getName().compare("Object") != 0 )
   {
      if ( !ast.hasBaseType() )
      {
         ASTType* ptype = new ASTType(ASTType::eObject);
         ptype->setObjectName("System.Object");
         ast.setBaseType(ptype);
      }

      resolveType(ast.getBaseType());
   }

   ASTTypeList& intrfaces = ast.getInterfaces();
   for ( int index = 0; index < intrfaces.size(); index++ )
   {
      ASTType& type = intrfaces[index];
      resolveType(type);
   }

   if ( !ast.hasConstructor() )
   {
      createDefaultConstructor(ast);
   }

   visitChildren(ast);
}

void SymbolCollectorVisitor::visit(ASTFunction& ast)
{
   resolveType(ast.getType());

   visitChildren(ast); // <-- arguments

   if ( ast.hasBody() )
   {
      ast.getBody().accept(*this);
   }
   else if ( !ast.getModifiers().isAbstract() && !ast.getModifiers().isNative() )
   {
      mContext.getLog().error("Function " + mpClass->getName() + "." + ast.getName() + " requires a body.");
   }
}

void SymbolCollectorVisitor::visit(ASTFunctionArgument& ast)
{
   ASTVariable& var = ast.getVariable();

   resolveType(var.getType());

   if ( var.hasExpression() )
   {
      var.getExpression().accept(*this);
   }
}

void SymbolCollectorVisitor::visit(ASTField& ast)
{
   ASTVariable& var = ast.getVariable();

   resolveType(var.getType());

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

   resolveType(var.getType());

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

   resolveType(var.getType());

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

void SymbolCollectorVisitor::visit(ASTSwitch& ast)
{
   ast.getExpression().accept(*this);

   visitChildren(ast);
}

void SymbolCollectorVisitor::visit(ASTCase& ast)
{
   ast.getBody().accept(*this);
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

void SymbolCollectorVisitor::visit(ASTInstanceOf& ast)
{
   resolveType(ast.getInstanceType());

   ast.getObject().accept(*this);
}

void SymbolCollectorVisitor::visit(ASTNew& ast)
{
   resolveType(ast.getType());

   visitChildren(ast);
}

void SymbolCollectorVisitor::visit(ASTCast& ast)
{
   resolveType(ast.getType());
   
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
   ASTUnary* punary = new ASTUnary();
   punary->addPart(psuper);
   ASTExpression* pexpression = new ASTExpression();
   pexpression->setLeft(punary);
   ASTExpressionStatement* pexprstmt = new ASTExpressionStatement(pexpression);
   pbody->addChild(pexprstmt);
   
   ASTFunction* pconstructor = new ASTFunction(ASTMember::eConstructor);
   pconstructor->setName(ast.getName());
   pconstructor->setBody(pbody);
   pconstructor->setType(new ASTType(ASTType::eVoid));

   ast.addMember(pconstructor);
}

/// Resolve the type, if resolving fails, a message is logged
void SymbolCollectorVisitor::resolveType(ASTType& type)
{
   if ( !type.resolveType(mContext, *mpClass) )
   {
      mContext.getLog().error("Can not resolve type " + type.toString());
   }
}
