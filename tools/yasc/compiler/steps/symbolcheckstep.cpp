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
#include "symbolcheckstep.h"

#include "core/defines.h"
#include "core/smartptr/scopedvalue.h"
#include "core/conv/numberconverter.h"

#include "yasc/compiler/errornumbers.h"
#include "yasc/compiler/ast/ast.h"
#include "yasc/compiler/scope/scope.h"
#include "yasc/compiler/scope/scopevariable.h"
#include "yasc/compiler/scope/scopedscope.h"
#include "yasc/compiler/compilecontext.h"
#include "yasc/compiler/classresolver.h"
#include "yasc/common/literal.h"

#include "variablecheckvisitor.h"

/*

ScopedTable :
- when entering a block add push scope
- leaving block pop scope

Member variables are at top level scope
When variable declared, they are added to the current scope.
*/

SymbolCheckVisitor::SymbolCheckVisitor(CompileContext& context):
   CompileStep(context),
   mpFunction(NULL),
   mCurrentType(),
   mScopeStack(),
   mStatic(false)
{
}

// - Overloads

bool SymbolCheckVisitor::performStep(ASTNode& node)
{
   node.accept(*this);
   return true;
}

// - Visiting

void SymbolCheckVisitor::visit(ASTRoot& root)
{
   visitChildren(root);
}

void SymbolCheckVisitor::visit(ASTClass& ast)
{
   ScopedScope scope(mScopeStack);

   mpClass = &ast;

   visitChildren(ast);
}

void SymbolCheckVisitor::visit(ASTFunction& ast)
{
   ScopedScope scope(mScopeStack);
   ScopedValue<ASTFunction*> scopedfunction(&mpFunction, &ast, mpFunction);
   ASSERT_PTR(mpFunction);

   visitChildren(ast); // <-- arguments

   if ( ast.isConstructor() && ast.getName() != mpClass->getName() )
   {
      error(E0005, UTEXT("Function ") + ast.getName() + UTEXT(" must have a return type (or equal class name as constructor)."), ast);
   }

   if ( ast.hasBody() )
   {
      if ( ast.getModifiers().isAbstract() )
      {
         error(E0006, UTEXT("Abstract function ") + ast.getName() + UTEXT(" should not have a body."), ast);
      }
      else
      {
         checkReturn(ast);

         ast.getBody().accept(*this);
      }
   }
   else if ( !ast.getModifiers().isAbstract() && !ast.getModifiers().isPureNative() )
   {
      error(E0007, UTEXT("Function ") + ast.getName() + UTEXT(" requires a body."), ast);
   }

   mCurrentType.clear();
}

void SymbolCheckVisitor::visit(ASTFunctionArgument& ast)
{
   ASTVariable& var = ast.getVariable();
   checkVarInit(var);
   
   ScopeVariable* pvariable = ScopeVariable::fromVariable(var);
   mScopeStack.add(pvariable);
}

void SymbolCheckVisitor::visit(ASTField& ast)
{
   ASTVariable& var = ast.getVariable();
   checkUnknown(var.getType());
   checkVarInit(var);
}

void SymbolCheckVisitor::visit(ASTProperty& ast)
{
   checkUnknown(ast.getType());
}

void SymbolCheckVisitor::visit(ASTVariableInit& ast)
{
   visitChildren(ast);
}

void SymbolCheckVisitor::visit(ASTArrayInit& ast)
{
   visitChildren(ast);

   ASTType elementtype = mCurrentType;

   mCurrentType.setKind(ASTType::eArray);
   mCurrentType.setArrayType(elementtype.clone());
   mCurrentType.setArrayDimension(1); // need to determine the depth of the array!
}

void SymbolCheckVisitor::visit(ASTBlock& ast)
{
   ScopedScope scope(mScopeStack);

   visitChildren(ast);
}

void SymbolCheckVisitor::visit(ASTLocalVariable& ast)
{
   ASTVariable& var = ast.getVariable();
   checkUnknown(var.getType());
   checkVarInit(var);

   mpFunction->addLocal(var.getType().clone());

   ScopeVariable* pvariable = ScopeVariable::fromVariable(ast.getVariable());
   mScopeStack.add(pvariable);
}

void SymbolCheckVisitor::visit(ASTExpressionStatement& ast)
{
   ast.getExpression().accept(*this);   
}

void SymbolCheckVisitor::visit(ASTIf& ast)
{
   ast.getCondition().accept(*this);
   checkCondition(ast, mCurrentType);

   ast.getStatement().accept(*this);

   if ( ast.hasElseStatement() )
   {
      ast.getElseStatement().accept(*this);
   }
}

void SymbolCheckVisitor::visit(ASTFor& ast)
{
   ScopedScope scope(mScopeStack);

   if ( ast.hasInitializer() )
   {
      ast.getInitializer().accept(*this);
   }

   if ( ast.hasCondition() )
   {
      ast.getCondition().accept(*this);
      checkCondition(ast, mCurrentType);
   }

   visitChildren(ast); // <-- loop expressions

   ast.getBody().accept(*this);
}

void SymbolCheckVisitor::visit(ASTForeach& ast)
{
   ScopedScope scope(mScopeStack);

   ASTVariable& iteratorvar = ast.getIteratorVariable();
   ASTVariable& var = ast.getVariable();

   if ( var.hasInit() )
   {
      ASTVariableInit& varinit = var.getInit();
      varinit.getExpression().accept(*this);

      ASTClass& iterableclass = mContext.resolveClass(UTEXT("engine.collections.Iterable"));
      ASTType* piteratortype = new ASTType();

      if ( mCurrentType.isObject() )
      {
         if( mCurrentType.getObjectClass().isImplementing(iterableclass) )
         {
            piteratortype->setKind(ASTType::eObject);
            piteratortype->setObjectClass(iterableclass);
         }
         else
         {
            error(E0009, UTEXT("Container ") + var.getName() + UTEXT(" must be iterable for use in foreach."), ast);
         }
      }
      else if ( mCurrentType.isArray() )
      {
         piteratortype->setKind(ASTType::eInt);
      }

      iteratorvar.setType(piteratortype);
   }
   else
   {
      error(E0010, UTEXT("Missing required initializer for foreach variable ") + var.getName(), ast);
   }

   mpFunction->addLocal(iteratorvar.getType().clone());
   mpFunction->addLocal(var.getType().clone());

   ScopeVariable* pvariable = ScopeVariable::fromVariable(var);
   mScopeStack.add(pvariable);

   ast.getBody().accept(*this);
}

void SymbolCheckVisitor::visit(ASTWhile& ast)
{
   ast.getCondition().accept(*this);
   checkCondition(ast, mCurrentType);

   ast.getBody().accept(*this);
}

void SymbolCheckVisitor::visit(ASTDo& ast)
{
   ast.getBody().accept(*this);
   ast.getCondition().accept(*this);
   checkCondition(ast, mCurrentType);
}

void SymbolCheckVisitor::visit(ASTSwitch& ast)
{
   ast.getExpression().accept(*this);
   ast.setType(mCurrentType.clone());

   if ( !mCurrentType.isValueType() )
   {
      error(E0011, UTEXT("Switch statement value should be a value, found ") + mCurrentType.toString(), ast);
   }

   if ( ast.getDefaultCount() > 1 )
   {
      error(E0012, UTEXT("A switch statement can have only one default case."), ast);
   }

   visitChildren(ast);

   checkCaseTypes(ast);
}

void SymbolCheckVisitor::visit(ASTCase& ast)
{
   mCurrentType.clear();

   if ( !ast.isDefault() )
   {
      ast.getValueExpression().accept(*this);
      ast.setType(mCurrentType.clone());

      // convert the value expression to a value for the lookup table
      ASTUnary& expressionnode = ast.getValueExpression();
      const ASTLiteral* pliteral = dynamic_cast<const ASTLiteral*>(&expressionnode.getParts()[0]);
      if ( pliteral != NULL )
      {
         // we can use a lookup table for fast lookup
         ast.setValue(pliteral->getLiteral().getValue());
      }
      else
      {
         // code generator will insert if statements to compare with the values
      }
   }

   ast.getBody().accept(*this);
}

void SymbolCheckVisitor::visit(ASTReturn& ast)
{
   if ( ast.hasExpression() )
   {
      if ( mpFunction->getType().isVoid() )
      {
         error(E0013, UTEXT("Void functions can not return an object or value."), ast);
      }
      else
      {
         ast.getExpression().accept(*this);

         if ( !mCurrentType.greater(mpFunction->getType()) )
         {
            error(E0014, UTEXT("Can not return value of type ") + mCurrentType.toString() + UTEXT(", it is not derived from ") + mpFunction->getType().toString(), ast);
         }
      }
   }
   else if ( !mpFunction->getType().isVoid() )
   {
      error(E0015, UTEXT("Return a value of type ") + mpFunction->getType().toString(), ast);
   }
}

void SymbolCheckVisitor::visit(ASTTry& ast)
{
   ast.getBody().accept(*this);

   if ( ast.hasFinallyBlock() )
   {
      ast.getFinallyBlock().accept(*this);
   }

   visitChildren(ast);
}

void SymbolCheckVisitor::visit(ASTCatch& ast)
{
   ast.getVariable().accept(*this);

   const ASTVariable& var = ast.getVariable().getVariable();

   const ASTType& type = var.getType();
   bool ok = type.isObject();
   if ( ok )
   {
      const ASTClass& throwable = mContext.resolveClass(UTEXT("system.Throwable"));
      if ( !var.getType().getObjectClass().isBase(throwable) )
      {
         ok = false;
      }
   }

   if ( !ok )
   {
      error(E0016, UTEXT("Catch expects a throwable object."), ast);
   }

   ast.getBody().accept(*this);
}

void SymbolCheckVisitor::visit(ASTThrow& ast)
{
   ast.getExpression().accept(*this);

   if ( mCurrentType.isObject() )
   {
      const ASTClass* pthrowable = mContext.findClass(UTEXT("system.Throwable"));
      if ( !mCurrentType.getObjectClass().isBase(*pthrowable) )
      {
         error(E0017, UTEXT("Throw argument must be derived from Throwable."), ast);
      }
   }
   else
   {
      error(E0018, UTEXT("Throw requires an expression resulting in a throwable object."), ast);
   }
}

void SymbolCheckVisitor::visit(ASTAssert& ast)
{
   mCurrentType.clear();

   ast.getCondition().accept(*this);

   if ( !mCurrentType.isBoolean() )
   {
      error(E0019, UTEXT("The assert condition expression must be of type boolean."), ast);
   }
}

void SymbolCheckVisitor::visit(ASTExpression& ast)
{
   mCurrentType.clear();

   ast.getLeft().accept(*this);

   if ( ast.hasRight() )
   {
      ASTType lefttype = mCurrentType;

      mCurrentType.clear();

      if ( isVariable(ast.getLeft()) )
      {
         ast.getRight().accept(*this);

         if ( !mCurrentType.greater(lefttype) )
         {
            error(E0020, UTEXT("Invalid type for assignment. Can not assign ") + mCurrentType.toString() + UTEXT(" to ") + lefttype.toString(), ast);
         }
      }
      else
      {
         error(E0021, UTEXT("Can only assign to variables."), ast);
      }
   }
}

void SymbolCheckVisitor::visit(ASTConcatenate& ast)
{
   ast.getLeft().accept(*this);

   ASTType lefttype = mCurrentType;

   mCurrentType.clear();

   ast.getRight().accept(*this);

   switch ( ast.getMode() )
   {
      case ASTConcatenate::eMul:
      case ASTConcatenate::eDiv:
      case ASTConcatenate::eRem:
      case ASTConcatenate::ePlus:
      case ASTConcatenate::eMinus:
         {
            ASTType righttype = mCurrentType;

            mCurrentType = ASTType::greaterType(lefttype, righttype);
            if ( mCurrentType.isValid() )
            {
               // optionally add casts if necessary

               if ( mCurrentType.isChar() && lefttype.isString() )
               {
                  // add char is now allowed directly:
                  //  str = str + char  <- no casts
                  //  str = char + str  <- casts char to string
                  break;
               }

               if ( !lefttype.equals(mCurrentType) )
               {
                  ASTCast* pcast = new ASTCast();
                  pcast->setType(mCurrentType.clone());
                  pcast->setNode(ast.useLeft());

                  ast.setLeft(pcast);
               }
               if ( !righttype.equals(mCurrentType) )
               {
                  ASTCast* pcast = new ASTCast();
                  pcast->setType(mCurrentType.clone());
                  pcast->setNode(ast.useRight());

                  ast.setRight(pcast);
               }
            }
            else
            {
               String op = UTEXT("+");
               error(E0022, UTEXT("Can not execute operator ") + op + UTEXT(" on types ") + lefttype.toString() + UTEXT(" and ") + righttype.toString(), ast);
            }
         }
         break;

      case ASTConcatenate::eBitwiseOr:
      case ASTConcatenate::eBitwiseXor:
      case ASTConcatenate::eBitwiseAnd:
      case ASTConcatenate::eShiftLeft:
      case ASTConcatenate::eShiftRight:
         {
            ASTType righttype = mCurrentType;

            if ( !lefttype.isInt() || !righttype.isInt() )
            {
               error(E0023, UTEXT("Bitwise operators only operate on int values."), ast);
            }
         }
         break;

      case ASTConcatenate::eAnd:
      case ASTConcatenate::eOr:
         {
            if ( !lefttype.isBoolean() || !mCurrentType.isBoolean() )
            {
               String op = UTEXT("&&"); // add toString to Mode
               error(E0024, UTEXT("Operator ") + op + UTEXT(" requires boolean expressions."), ast);
            }
         }
         break;

      case ASTConcatenate::eEquals:
      case ASTConcatenate::eUnequals:
      case ASTConcatenate::eSmallerEqual:
      case ASTConcatenate::eSmaller:
      case ASTConcatenate::eGreater:
      case ASTConcatenate::eGreaterEqual:
         {
            ASTType comp = ASTType::greaterType(lefttype, mCurrentType);
            if ( !comp.isValid() )
            {
               error(E0025, UTEXT("Can not compare ") + lefttype.toString() + UTEXT(" with ") + mCurrentType.toString(), ast);
            }
            else if ( ast.getMode() >= ASTConcatenate::eSmallerEqual )
            {
               if ( comp.isObject() || comp.isArray() || comp.isBoolean() )
               {
                  error(E0026, UTEXT("Invalid type ") + comp.toString() + UTEXT(" for operator."), ast);
               }
            }

            mCurrentType = ASTType(ASTType::eBoolean);
         }
         break;
         
      case ASTConcatenate::eInvalid:
         error(E0001, UTEXT("Invalid compiler state!"), ast);
         break;
   }
}

void SymbolCheckVisitor::visit(ASTUnary& ast)
{
   visitChildren(ast);

   checkOperator(ast, ast.getPre());
   checkOperator(ast, ast.getPost());
}

void SymbolCheckVisitor::visit(ASTInstanceOf& ast)
{
   mCurrentType.clear();

   checkUnknown(ast.getInstanceType());

   ast.getObject().accept(*this);

   if ( !(mCurrentType.isObject() || mCurrentType.isArray()) )
   {
      error(E0027, UTEXT("Operator instanceof can only be called against objects/arrays."), ast);
   }
   else if ( !mCurrentType.isDerivedFrom(ast.getInstanceType()) )
   {
      error(E0028, UTEXT("Instanceof operator can never be true for ") + mCurrentType.toString() + UTEXT(" and ") + ast.getInstanceType().toString(), ast);
   }

   mCurrentType = ASTType(ASTType::eBoolean);
}

void SymbolCheckVisitor::visit(ASTNew& ast)
{
   switch ( ast.getKind() )
   {
      case ASTNew::eObject:
         {
            ASTType before = mCurrentType;

            ASTSignature signature;
            ASTNodes& arguments = ast.getArguments();
            for ( int index = 0; index < arguments.size(); index++ )
            {
               ASTExpression& expr = dynamic_cast<ASTExpression&>(arguments[index]);
               expr.accept(*this);

               signature.append(mCurrentType.clone());
            }

            checkUnknown(ast.getType());

            if ( ast.getType().hasObjectClass() )
            {
               const ASTClass& newclass = ast.getType().getObjectClass();
               const ASTFunction* pfunction = newclass.findBestMatch(newclass.getName(), signature, before.getTypeArguments());

               if ( pfunction == NULL )
               {
                  String arguments = UTEXT("(") + signature.toString() + ')';
                  error(E0029, UTEXT("No matching constructor ") + newclass.getFullName() + arguments + UTEXT(" defined."), ast);
               }
               else
               {
                  ast.setConstructor(*pfunction);
               }
            }

            mCurrentType = ast.getType();
         }
         break;

      case ASTNew::eArray:
         {
            ASTNodes& arguments = ast.getArguments();
            for ( int index = 0; index < arguments.size(); index++ )
            {
               ASTExpression& expr = dynamic_cast<ASTExpression&>(arguments[index]);
               expr.accept(*this);

               if ( !mCurrentType.isInt() )
               {
                  error(E0030, UTEXT("Array size expression should be of type int."), ast);
               }
            }

            mCurrentType = ast.getType();
         }
         break;
         
      case ASTNew::eInvalid:
         error(E0001, UTEXT("Invalid compiler state!"), ast);
         break;
   }
}

void SymbolCheckVisitor::visit(ASTCast& ast)
{
   ast.getNode().accept(*this);

   ASTType nodetype = mCurrentType;

   mCurrentType = ast.getType();

   // check if nodetype can be cast to mCurrentType
}

void SymbolCheckVisitor::visit(ASTSuper& ast)
{
   ASSERT_PTR(mpClass);
   ASSERT_PTR(mpFunction);

   if ( mpClass->hasBaseType() && mpClass->getBaseType().isUnknown() )
   {
      // skip calls to the unknown super class
      return;
   }

   if ( mpFunction->getModifiers().isStatic() )
   {
      // statics do not have a this (thus also not a super)
      String str = ast.getKind() == ASTSuper::eThis ? UTEXT("this") : UTEXT("super");
      error(E0031, UTEXT("Can not access ") + str + UTEXT(" from a static function."), ast);
      return;
   }

   ASTClass* pclass = ast.isThis() ? mpClass : &mpClass->getBaseClass();

   if ( ast.isCall() )
   {
      ASTSignature signature;
      ASTNodes& arguments = ast.getArguments();
      for ( int index = 0; index < arguments.size(); index++ )
      {
         ASTExpression& expr = dynamic_cast<ASTExpression&>(arguments[index]);
         expr.accept(*this);

         signature.append(mCurrentType.clone());
      }

      ASTTypeList types;
      ASTFunction* pconstructor = pclass->findBestMatch(pclass->getName(), signature, types);
      if ( pconstructor != NULL )
      {
         ast.setConstructor(pconstructor);
      }
      else
      {
         String arguments = String("(") + signature.toString() + ')';
         error(E0032, UTEXT("No constructor ") + pclass->getFullName() + arguments + UTEXT(" defined."), ast);
      }

      // calls to this and super return nothing (void)
      mCurrentType = ASTType::SVoidType;
   }
   else
   {
      // note: pclass already is correct class for this or super

      mCurrentType.setKind(ASTType::eObject);
      mCurrentType.setObjectName(pclass->getFullName());
      mCurrentType.setObjectClass(*pclass);
   }
}

void SymbolCheckVisitor::visit(ASTNative& ast)
{
   if ( ast.hasArguments() )
   {
      ASSERT_PTR(mpFunction);
      const ASTSignature& signature = mpFunction->getSignature();

      ASTNodes& arguments = ast.getArguments();
      for ( int index = 0; index < arguments.size(); index++ )
      {
         ASTExpression& expr = dynamic_cast<ASTExpression&>(arguments[index]);
         expr.accept(*this);

         const ASTType& sigtype = signature[index];
         if ( !mCurrentType.greater(sigtype) )
         {
            String sidx;
            NumberConverter::getInstance().format(sidx, index);
            String func = mpClass->getFullName() + '.' + mpFunction->getName();
            error(E0033, UTEXT("While calling native implementation of function ")
                         + func
                         + UTEXT("; argument ")
                         + sidx
                         + UTEXT(" of can not be mapped from ")
                         + mCurrentType.toString()
                         + UTEXT(" to ")
                         + sigtype.toString(), ast);
         }
      }

      ASTTypeList types;
      if ( !mpFunction->getSignature().bestMatch(signature, types) )
      {
         String arguments = signature.toString();
         error(E0034, UTEXT("Arguments to native do not match ") + arguments, ast);
      }
   }

   ASSERT_PTR(mpFunction);
   mCurrentType = mpFunction->getType();
}

void SymbolCheckVisitor::visit(ASTLiteral& ast)
{
   mCurrentType = ast.getType();
}

void SymbolCheckVisitor::visit(ASTAccess& ast)
{
   const String& name = ast.getName();
   bool wasstatic = mStatic;
   mStatic = false;

   switch ( ast.getKind() )
   {
      case ASTAccess::eVariable:
         {
            if ( mCurrentType.isValid() )
            {
               const ASTClass& aclass = mCurrentType.getObjectClass();
               const ASTField* pfield = NULL;

               if ( wasstatic )
               {
                  pfield = aclass.findStatic(name);
               }
               else
               {
                  pfield = aclass.findField(name); // <-- here type of variable can be a generic, so map it to the declaration
               }

               if ( pfield != NULL )
               {
                  // variable access of reference class

                  const ASTVariable& var = pfield->getVariable();

                  ast.setAccess(ASTAccess::eRefField);
                  ast.setField(*pfield);

                  const ASTType& type = var.getType();
                  if ( type.isGeneric() )
                  {
                     const ASTTypeVariable& typevariable = type.getTypeVariable();
                     if ( ast.hasTypeArguments() )
                     {
                        mCurrentType = ast.getTypeArguments()[typevariable.getIndex()];
                     }
                     else
                     {
                        mCurrentType = mCurrentType.getTypeArguments()[typevariable.getIndex()];
                     }
                  }
                  else
                  {
                     mCurrentType = type;
                  }
               }
               else
               {
                  error(E0035, UTEXT("Class ") + aclass.getName() + UTEXT(" has no member variable ") + name, ast);
               }
            }
            else
            {
               //
               // local variable
               const ScopeVariable* pvariable = mScopeStack.find(name);
               if ( pvariable != NULL )
               {
                  const ASTVariable& var = pvariable->getVariable();

                  ast.setAccess(var.isArgument() ? ASTAccess::eArgument : ASTAccess::eLocal);
                  ast.setVariable(pvariable->getVariable());

                  mCurrentType = pvariable->getType();
               }
               else
               {
                  // variable access on own class or local variable
                  ASTField* pfield = mpClass->findField(name);
                  if ( pfield == NULL )
                     pfield = mpClass->findStatic(name);

                  if ( pfield != NULL )
                  {
                     const ASTVariable& var = pfield->getVariable();

                     // if in a function (not the case for member intialization) check if we aren't accessing
                     // none static members from a static function.
                     if ( mpFunction != NULL && (mpFunction->getModifiers().isStatic() && !var.getModifiers().isStatic()) )
                     {
                        error(E0036, UTEXT("Can not access instance member ") + var.getName(), ast);
                     }

                     // variable access on current class
                     ast.setAccess(ASTAccess::eField);
                     ast.setField(*pfield);

                     mCurrentType = pfield->getVariable().getType();
                  }
                  else
                  {
                     error(E0037, UTEXT("Identifier ") + name + UTEXT(" is not defined."), ast);
                  }
               }
            }

            if ( wasstatic && !ast.getVariable().getModifiers().isStatic() )
            {
               error(E0038, UTEXT("Can not access non static variable ") + ast.getName() + UTEXT(" from static"), ast);
            }
         }
         break;

      case ASTAccess::eFunction:
         {
            if ( mCurrentType.isValid() )
            {
               switch ( mCurrentType.getKind() )
               {
                  case ASTType::eObject:
                     {
                        ASSERT(mCurrentType.hasObjectClass());
                        const ASTClass& type = mCurrentType.getObjectClass();
                        checkFunctionAccess(type, ast, wasstatic);
                     }
                     break;
                  case ASTType::eArray:
                     {
                        const ASTClass& arrayclass = mContext.resolveClass(UTEXT("system.InternalArray"));
                        checkFunctionAccess(arrayclass, ast, wasstatic);
                     }
                     break;
                  case ASTType::eString:
                     {
                        const ASTClass& stringclass = mContext.resolveClass(UTEXT("system.InternalString"));
                        checkFunctionAccess(stringclass, ast, wasstatic);
                     }
                     break;
                  case ASTType::eVoid:
                     {
                        error(E0039, UTEXT("Can not invoke a method on a void object."), ast);
                     }
                     break;
                  default:
                     {
                        error(E0040, UTEXT("Can not invoke method on basic types."), ast);
                     }
                     break;
               }
            }
            else
            {
               checkFunctionAccess(*mpClass, ast, wasstatic);
            }
         }
         break;

      case ASTAccess::eArray:
         {
            if ( !mCurrentType.isArray() )
            {
               error(E0041, mCurrentType.toString() + UTEXT(" is not an array type."), ast);
            }
            else
            {
               ast.setAccess(ASTAccess::eArrayAccess);

               ASTType before = mCurrentType;

               ASTNodes& arguments = ast.getArguments();
               for ( int index = 0; index < arguments.size(); index++ )
               {
                  ASTExpression& expr = dynamic_cast<ASTExpression&>(arguments[index]);
                  expr.accept(*this);

                  if ( !mCurrentType.isInt() )
                  {
                     error(E0042, UTEXT("Array access expression must be of type int."), ast);
                  }
               }

               mCurrentType = before.getArrayType();
            }
         }
         break;

      case ASTAccess::eStatic:
         mCurrentType = ast.getStaticType();
         mStatic = true;
         break;

      case ASTAccess::eClass:
         {
            if ( !mCurrentType.isObject() )
            {
               warning(W0002, UTEXT("The class operator currently is only supported for objects."), ast);
            }

            if ( !wasstatic )
               ast.setAccess(ASTAccess::eField);
            else
               ast.setAccess(ASTAccess::eRefField);

            mCurrentType.clear();
            mCurrentType.setKind(ASTType::eObject);
            mCurrentType.setObjectName(UTEXT("system.Class"));
            mCurrentType.setObjectClass(mContext.resolveClass(UTEXT("system.Class")));
         }
         break;

      case ASTAccess::eInvalid:
      default:
         {
            error(E0043, UTEXT("Unknown access detected."), ast);
         }
         break;
   }
}

void SymbolCheckVisitor::visit(ASTCompound& compound)
{
   compound.getExpression().accept(*this);
}

// - Query

bool SymbolCheckVisitor::isVariable(const ASTNode& node) const
{
   return VariableCheckVisitor::isVariable(node);
}

// - Operations

void SymbolCheckVisitor::checkVarInit(ASTVariable& var)
{
   if ( var.hasInit() )
   {
      ASTVariableInit& varinit = var.getInit();

      if ( varinit.hasExpression() )
      {
         varinit.getExpression().accept(*this);
      }
      else
      {
         varinit.getArrayInit().accept(*this);
      }

      if ( !mCurrentType.greater(var.getType()) )
      {
         error(E0044,
            UTEXT("Assigning wrong type to variable '")
            + var.getName() 
            + UTEXT("' was expecting ") 
            + var.getType().toString() 
            + UTEXT(" and got ") 
            + mCurrentType.toString(), var.getPosition());
      }
   }
}

void SymbolCheckVisitor::checkCondition(const ASTNode& node, const ASTType& type)
{
   if ( !type.isBoolean() )
   {
      error(E0008, UTEXT("Condition expression must return a boolean value."), node);
   }
}

void SymbolCheckVisitor::checkReturn(const ASTFunction& function)
{
   checkUnknown(function.getType());

   if ( !function.getType().isVoid() )
   {
      // ensure that we have a return statement
      bool hasunreachablecode = false;
      if ( !function.getBody().hasReturn(hasunreachablecode) )
      {
         error(E0045, UTEXT("Function ") + function.getName() + UTEXT(" should return a value of type ") + function.getType().toString(), function.getPosition());
      }
      else if ( hasunreachablecode )
      {
         warning(W0003, UTEXT("Unreachable code in ") + function.getName(), function.getPosition());
      }
   }
}

void SymbolCheckVisitor::checkCaseTypes(const ASTSwitch& ast)
{
   const ASTType& casetype = ast.getType();
   int total = ast.getTotalCount();
   for ( int index = 0; index < total; index++ )
   {
      const ASTCase& astcase = ast.getCase(index);
      if ( astcase.isCase() && !casetype.greater(astcase.getType()) )
      {
         String ss = UTEXT("Case {0} should be of type {1}.").arg(0, index).arg(1, casetype.toString());
         error(E0059, ss, ast);
      }
   }
}

void SymbolCheckVisitor::checkFunctionAccess(const ASTClass& klass, ASTAccess& access, bool isstatic)
{
   ASTType before = mCurrentType;

   ASTSignature signature;
   ASTNodes& arguments = access.getArguments();
   for ( int index = 0; index < arguments.size(); index++ )
   {
      ASTExpression& expr = dynamic_cast<ASTExpression&>(arguments[index]);
      expr.accept(*this);

      signature.append(mCurrentType.clone());
   }

   const ASTTypeList* ptypelist = NULL;
   if ( access.hasTypeArguments() )
      ptypelist = &access.getTypeArguments();
   else
      ptypelist = &before.getTypeArguments();

   const ASTFunction* pfunction = klass.findBestMatch(access.getName(), signature, *ptypelist);

   if ( pfunction != NULL )
   {
      const ASTSignature& funcsig = pfunction->getSignature();

      // check if cast is required
      for ( int index = 0; index < signature.size(); index++ )
      {
         const ASTType& type = signature[index];
         const ASTType& fnctype = funcsig[index];

         if ( !fnctype.isGeneric() && !type.equals(fnctype) )
         {
            ASTCast* pcast = new ASTCast();
            pcast->setType(fnctype.clone());
            pcast->setNode(&access.getArguments()[index]);

            access.replaceArgument(index, pcast);
         }
      }

      if ( isstatic && !pfunction->getModifiers().isStatic() )
      {
         error(E0047, UTEXT("Can not call non static function ") + pfunction->getName(), access);
      }
      access.setFunction(*pfunction);

      const ASTType& type = pfunction->getType();
      if ( type.isGeneric() )
      {
         const ASTTypeVariable& typevariable = type.getTypeVariable();
         mCurrentType = before.getTypeArguments()[typevariable.getIndex()];
      }
      else if ( !type.getTypeArguments().empty() )
      {
         mCurrentType = pfunction->getType();

         // if we get a generic type argument, replace it with the actual variable.
         const ASTType& arg = type.getTypeArguments()[0];
         if ( arg.isGeneric() )
         {
            const ASTTypeVariable* pvariable = klass.getTypeVariables().find(arg.getObjectName());
            if ( pvariable != NULL )
            {
               mCurrentType.replaceArgument(before.getTypeArguments()[pvariable->getIndex()]);
            }
         }
      }
      else
      {
         mCurrentType = pfunction->getType();
      }
   }
   else
   {
      String arguments = UTEXT("(") + signature.toString() + ')';
      error(E0048, UTEXT("No matching function ") + klass.getName() + '.' + access.getName() + arguments + UTEXT(" defined."), access);
   }
}

void SymbolCheckVisitor::checkOperator(ASTNode& node, ASTUnary::Operator op)
{
   switch ( op )
   {
      case ASTUnary::ePos:
      case ASTUnary::eNegate:
      case ASTUnary::eIncrement:
      case ASTUnary::eDecrement:
         {
            if ( !mCurrentType.isNumeric() )
            {
               error(E0049, UTEXT("Pre operator requires a numeric value."), node);
            }
         }
         break;
      case ASTUnary::eNot:
         {
            if ( !mCurrentType.isBoolean() )
            {
               error(E0050, UTEXT("Not operator requires a boolean value."), node);
            }
         }
         break;
         
      case ASTUnary::eNone:
         break;
      }
}

void SymbolCheckVisitor::checkUnknown(const ASTType& type)
{
   if ( type.isUnknown() )
   {
      error(E0051, UTEXT("Unknown class type ") + type.getObjectName(), type.getPosition());
   }
}
