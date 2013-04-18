/***************************************************************************
 *   Copyright (C) 2012 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "symbolcollectorstep.h"

#include "script/compiler/compilecontext.h"

#include "script/ast/ast.h"

SymbolCollectorVisitor::SymbolCollectorVisitor(CompileContext& context):
   CompileStep(),
   mContext(context),
   mResolver(),
   mpClass(NULL),
   mpFunction(NULL)
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

void SymbolCollectorVisitor::visit(ASTClass& ast)
{
   mpClass = &ast;

   if ( ast.hasBaseType() )
   {
      resolveType(ast.getBaseType());
   }

   ASTTypeList& intrfaces = ast.getInterfaces();
   for ( int index = 0; index < intrfaces.size(); index++ )
   {
      ASTType& type = intrfaces[index];
      resolveType(type);
   }

   if ( !ast.hasConstructor() && ast.hasBaseType() )
   {
      generateDefaultConstructor(ast);
   }

   visitChildren(ast);

   if ( ast.hasNativeConstructor() )
   {
      generateNativeFinalize(ast);
   }
}

void SymbolCollectorVisitor::visit(ASTFunction& ast)
{
   mpFunction = &ast;

   resolveType(ast.getType());

   visitChildren(ast); // <-- arguments

   if ( ast.isConstructor() && !ast.hasBody() )
   {
      if ( ast.getModifiers().isPureNative() )
      {
         // constructors should never be pure native!
         ast.getModifiers().setPureNative(false);
         ASSERT(ast.getModifiers().isNative());
      }

      // generate a default body, the super/native calls will be added later on when required
      generateConstructorBody(ast);
   }

   if ( ast.hasBody() )
   {
      mHasNativeCall = false;
      mHasSuperCall = false;

      ast.getBody().accept(*this);

      if ( ast.isDefaultConstructor() && !mHasSuperCall && mpClass->hasBaseType() )
      {
         generateSuperCall(ast);
      }
      
      if ( ast.getModifiers().isNative() && !mHasNativeCall )
      {
         generateNativeCall(ast);
      }
      else if ( mHasNativeCall )
      {
         ast.getModifiers().setNative(true);
      }
   }

   if ( ast.getType().isVoid() )
   {
      generateReturn(ast);
   }
}

void SymbolCollectorVisitor::visit(ASTFunctionArgument& ast)
{
   ASTVariable& var = ast.getVariable();

   resolveType(var.getType());

   if ( var.hasInit() )
   {
      var.getInit().accept(*this);
   }
}

void SymbolCollectorVisitor::visit(ASTField& ast)
{
   ASTVariable& var = ast.getVariable();

   resolveType(var.getType());

   if ( var.hasInit() )
   {
      var.getInit().accept(*this);
   }
}

void SymbolCollectorVisitor::visit(ASTVariableInit& ast)
{
   if ( ast.hasExpression() )
   {
      ast.getExpression().accept(*this);
   }
   else if ( ast.hasArrayInit() )
   {
      ast.getArrayInit().accept(*this);
   }
}

void SymbolCollectorVisitor::visit(ASTArrayInit& ast)
{
   visitChildren(ast);
}

void SymbolCollectorVisitor::visit(ASTBlock& ast)
{
   visitChildren(ast);
}

void SymbolCollectorVisitor::visit(ASTLocalVariable& ast)
{
   ASTVariable& var = ast.getVariable();

   resolveType(var.getType());

   if ( var.hasInit() )
   {
      var.getInit().accept(*this);
   }
}

void SymbolCollectorVisitor::visit(ASTExpressionStatement& ast)
{
   ast.getExpression().accept(*this);
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

   if ( var.hasInit() )
   {
      var.getInit().accept(*this);
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

void SymbolCollectorVisitor::visit(ASTAssert& ast)
{
   ast.getCondition().accept(*this);
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

void SymbolCollectorVisitor::visit(ASTSuper& ast)
{
   if ( ast.isCall() )
   {
      mHasSuperCall = true;

      ASTNodes& args = ast.getArguments();
      args.accept(*this);
   }
}

void SymbolCollectorVisitor::visit(ASTNative& ast)
{
   mHasNativeCall = true;
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

      case ASTAccess::eVariable:
      case ASTAccess::eStatic:
         {
         }
         break;
   }
}

void SymbolCollectorVisitor::visit(ASTCompound& compound)
{
   compound.getExpression().accept(*this);
}

// - Operations

void SymbolCollectorVisitor::generateDefaultConstructor(ASTClass& ast)
{
   ASTFunction* pconstructor = new ASTFunction(ASTMember::eConstructor);
   pconstructor->setName(ast.getName());
   pconstructor->setType(new ASTType(ASTType::eVoid));
   pconstructor->getModifiers().setVisibility(ASTModifiers::ePublic);

   ast.addMember(pconstructor);
}

void SymbolCollectorVisitor::generateNativeFinalize(ASTClass& ast)
{
   const FunctionRegistration* pdestructor = mContext.getClassRegistry().findCallback(ast, "destroy");
   if ( pdestructor != NULL )
   {
      ASTBlock* pbody = new ASTBlock();
      ASTNative* pnative = new ASTNative();
      pbody->addChild(pnative);

      ASTFunction* pfunc = new ASTFunction(ASTMember::eFunction);
      pfunc->setName(ast.getName());
      pfunc->setBody(pbody);
      pfunc->setType(new ASTType(ASTType::eVoid));

      ast.addMember(pfunc);
   }
}

void SymbolCollectorVisitor::generateConstructorBody(ASTFunction& ast)
{
   ASTBlock* pbody = new ASTBlock();
   ast.setBody(pbody);
}

void SymbolCollectorVisitor::generateSuperCall(ASTFunction& function)
{
   ASTSuper* psuper = new ASTSuper();
   psuper->setKind(ASTSuper::eSuper);
   psuper->setCall(true);
   ASTUnary* punary = new ASTUnary();
   punary->addPart(psuper);
   ASTExpression* pexpression = new ASTExpression();
   pexpression->setLeft(punary);
   ASTExpressionStatement* pexprstmt = new ASTExpressionStatement(pexpression);

   function.getBody().insertChild(0, pexprstmt);
}

void SymbolCollectorVisitor::generateNativeCall(ASTFunction& function)
{
   ASTNative* pnative = new ASTNative();
   ASTUnary* punary = new ASTUnary();
   punary->addPart(pnative);
   ASTExpression* pexpression = new ASTExpression();
   pexpression->setLeft(punary);
   ASTExpressionStatement* pexprstmt = new ASTExpressionStatement(pexpression);

   function.getBody().insertChild(0, pexprstmt);
}

void SymbolCollectorVisitor::generateReturn(ASTFunction& function)
{
   if ( !function.getModifiers().isAbstract() && !function.getModifiers().isPureNative() )
   {
      ASTReturn* preturn = new ASTReturn();
  
      function.getBody().addChild(preturn);
   }
}

/// Resolve the type, if resolving fails the type is marked as unknown so other 
/// compilation errors can be found earlier.
void SymbolCollectorVisitor::resolveType(ASTType& type)
{
   if ( !type.isVoid() )
   {
      const ASTTypeVariables* ptypevariables = NULL;
      if ( mpFunction != NULL && mpFunction->hasTypeVariables() )
      {
         ptypevariables = &mpFunction->getTypeVariables();
      }
      else if ( mpClass->isGeneric() )
      {
         ptypevariables = &mpClass->getTypeVariables();
      }

      if ( !type.resolveType(mContext, ptypevariables) )
      {
         mContext.getLog().error(String("Could not resolve type ") + type.toString());
         type.setKind(ASTType::eUnknown);
      }
   }
}
