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
#include "preloadvisitor.h"

#include "core/smartptr/scopedvalue.h"

#include "yasc/compiler/errornumbers.h"
#include "yasc/compiler/antlr/antlrexception.h"
#include "yasc/compiler/ast/ast.h"
#include "yasc/compiler/compiler.h"
#include "yasc/compiler/compilecontext.h"
#include "yasc/compiler/exceptions/classnotfoundexception.h"
#include "yasc/compiler/scope/scopedscope.h"
#include "yasc/compiler/scope/scopevariable.h"

PreloadVisitor::PreloadVisitor(CompileContext& context):
   CompileStep(context),
   mClassResolver(),
   mPackage(),
   mScopeStack(),
   mpFunction(NULL),
   mHasSuperCall(false),
   mHasNativeCall(false)
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

   mClassResolver.insert(mPackage + UTEXT(".*"));
}

void PreloadVisitor::visit(ASTUse& ast)
{
   mClassResolver.insert(ast.getIdentifier());
}

void PreloadVisitor::visit(ASTClass& ast)
{
   ScopedScope scope(mScopeStack);

   ScopedValue<ASTClass*> scopedclass(&mpClass, &ast, mpClass);
   mContext.addClass(&ast);

   mpClass = &ast;
   
   if ( ast.getName() != UTEXT("Object") )
   {
      if ( !ast.hasBaseType() )
      {
         ASTType* ptype = new ASTType(ASTType::eObject);
         ptype->setObjectName(UTEXT("system.Object"));
         ast.setBaseType(ptype);
      }

      ASSERT(ast.hasBaseType());
      if ( !load(ast.getBaseType()) )
      {
         error(E0001, UTEXT("Can not load base type ") + ast.getBaseType().getObjectName(), ast);
      }
      else
      {
         if ( !ast.getBaseType().isUnknown() )
         {
            ast.setBaseClass(mContext.resolveClass(ast.getBaseType().getObjectName()));
         }
      }
   }
   
   // register interfaces to the context, so it later can be resolved for the interface lookup
   // tables. these tables are links between interface function calls and the actuall virtual table
   // function entry.

   if ( ast.isInterface() )
   {
      mContext.addInterface(ast);
   }

   ASTTypeList& intrfaces = ast.getInterfaces();
   for ( int index = 0; index < intrfaces.size(); index++ )
   {
      ASTType& type = intrfaces[index];
      load(type);
   }

   ast.ensureDefaultConstructor();

   visitChildren(ast);

   ast.calculateResources();
}

void PreloadVisitor::visit(ASTFunction& ast)
{
   ScopedScope scope(mScopeStack);

   mpFunction = &ast;

   if ( !load(ast.getType()) )
   {
      // type is not known :(
   }

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

   visitChildren(ast); // <-- arguments

   if ( ast.hasBody() )
   {
      ScopedValue<bool> supercall(&mHasSuperCall, false, mHasSuperCall);
      ScopedValue<bool> nativecall(&mHasNativeCall, false, mHasNativeCall);

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

void PreloadVisitor::visit(ASTFunctionArgument& ast)
{
   ASTVariable& var = ast.getVariable();

   if ( !load(var.getType()) ) // <-- need to give class as param so the previous typevariable stuff can be moved there
   {
      // complain!
   }

   checkVarInit(var, false);
   
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

   checkVarInit(var, true);
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

   checkVarInit(var, true);

   ScopeVariable* pvariable = ScopeVariable::fromVariable(var);
   mScopeStack.add(pvariable);
}

void PreloadVisitor::visit(ASTExpressionStatement& ast)
{
   if ( ast.hasExpression() )
   {
      ast.getExpression().accept(*this);
   }
   else
   {
      error(E0002, UTEXT("Expression statement without expression."), ast);
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

   checkVarInit(var, false);

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
   if ( ast.hasValueExpression() )
   {
      ast.getValueExpression().accept(*this);
   }
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

void PreloadVisitor::visit(ASTAssert& ast)
{
   ast.getCondition().accept(*this);
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
   checkStaticAccess(ast);

   visitChildren(ast);
}

void PreloadVisitor::visit(ASTInstanceOf& ast)
{
   ast.getObject().accept(*this);

   load(ast.getInstanceType());
}

void PreloadVisitor::visit(ASTSuper& ast)
{
   if ( ast.isCall() )
   {
      mHasSuperCall = true;

      ASTNodes& args = ast.getArguments();
      args.accept(*this);
   }
}

void PreloadVisitor::visit(ASTNative& ast)
{
   mHasNativeCall = true;
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
      case ASTAccess::eClass:
         {
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
      const String& name = type.getObjectName();

      if ( name == UTEXT("engine.game.ContentManager") )
      {
         int aap  = 5;
      }

      ASTTypeList& args = type.getTypeArguments();
      for ( int index = 0; index < args.size(); ++index )
      {
         ASTType& argtype = args[index];
         load(argtype);
      }

      if ( mpClass->isTypeName(name) || (mpFunction != NULL && mpFunction->isTypeName(name)) )
      {
         // name is not a type for the typename of this generic class
      }
      else if ( !tryLoad(type) )
      {
         type.setKind(ASTType::eUnknown);
         return false;
      }
   }

   resolveType(type);

   return true;
}

bool PreloadVisitor::tryLoad(const ASTType& type)
{
   return mContext.loadClass(type.getObjectName());
}

/// Resolve the type, if resolving fails the type is marked as unknown so other 
/// compilation errors can be found earlier.
void PreloadVisitor::resolveType(ASTType& type)
{
   if ( !type.isVoid() && !type.isUnknown() )
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
         error(E0003, UTEXT("Could not resolve type ") + type.toString(), type.getPosition());
         type.setKind(ASTType::eUnknown);
      }
   }
}

// - Operations

void PreloadVisitor::checkStaticAccess(ASTUnary& unary)
{
   ASTAccess* paccess = dynamic_cast<ASTAccess*>(&unary.getParts()[0]);
   if ( paccess != NULL )
   {
      if ( mpClass->isMember(paccess->getName()) )
      {
         // it's a member field, so nothing to do here
         return;
      }

      ScopeVariable* pvariable = mScopeStack.find(paccess->getName());
      if ( pvariable == NULL )
      {
         String name;
         String qualifiedname;
         ASTAccess* pcurrent = paccess;
         ASTType type(ASTType::eObject);
         int count = 0;
         bool done = false;

         do
         {
            name += pcurrent->getName();

            // build full path
            qualifiedname = mClassResolver.resolve(name);
            if ( qualifiedname.length() == 0 )
            {
               qualifiedname = name;
            }

            // not a variable, so see if it is a static class
            type.setObjectName(qualifiedname);

            done = tryLoad(type);
            if ( !done )
            {
               pcurrent = pcurrent->hasNext() ? &pcurrent->getNext() : NULL;
               name += '.';
               count++;
            }
         }
         while ( !done && pcurrent != NULL );

         if ( done )
         {
            if ( pcurrent != paccess )
            {
               // add to class path
               std::size_t pos = qualifiedname.lastIndexOf('.');
               if ( pos != -1 )
               {
                  String path = qualifiedname.subStr(0, pos) + UTEXT(".*");
                  mClassResolver.insert(path);
               }

               // remove the path nodes
               ASTNodes& nodes = unary.getParts();
               nodes.erase(0, count);
            }

            ASTType* ptype = new ASTType(ASTType::eObject);
            ptype->setObjectName(pcurrent->getName());
            ptype->setObjectClass(mContext.resolveClass(qualifiedname));

            // make it a static access
            pcurrent->setName(qualifiedname);
            pcurrent->setKind(ASTAccess::eStatic);
            pcurrent->setStaticType(ptype);
         }
      }
   }
}

void PreloadVisitor::checkVarInit(ASTVariable& var, bool allowarray)
{
   if ( var.hasInit() )
   {
      ASTVariableInit& varinit = var.getInit();

      if ( varinit.hasArrayInit() )
      {
         if ( !allowarray )
         {
            error(E0004, UTEXT("It's not allowed to initialize variable ") + var.getName() + UTEXT(" with an array."), var.getPosition());
         }
         else
         {
            varinit.getArrayInit().accept(*this);
         }
      }
      else if ( varinit.hasExpression() )
      {
         varinit.getExpression().accept(*this);
      }
      else
      {
         warning(W0001, UTEXT("Compiler warning: variable ") + var.getName() + UTEXT(" has an empty initializer object."), var.getPosition());
      }
   }
}

// - Code generation

void PreloadVisitor::generateConstructorBody(ASTFunction& ast)
{
   ASTBlock* pbody = new ASTBlock();
   ast.setBody(pbody);
}

void PreloadVisitor::generateSuperCall(ASTFunction& function)
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

void PreloadVisitor::generateNativeCall(ASTFunction& function)
{
   ASTNative* pnative = new ASTNative();
   ASTUnary* punary = new ASTUnary();
   punary->addPart(pnative);
   ASTExpression* pexpression = new ASTExpression();
   pexpression->setLeft(punary);
   ASTExpressionStatement* pexprstmt = new ASTExpressionStatement(pexpression);

   function.getBody().insertChild(0, pexprstmt);
}

void PreloadVisitor::generateReturn(ASTFunction& function)
{
   if ( !function.getModifiers().isAbstract() && !function.getModifiers().isPureNative() )
   {
      ASTReturn* preturn = new ASTReturn();
  
      function.getBody().addChild(preturn);
   }
}
