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

#include "script/ast/ast.h"
#include "script/scope/scope.h"
#include "script/scope/scopevariable.h"
#include "script/scope/scopedscope.h"
#include "script/common/literal.h"
#include "script/compiler/compilecontext.h"
#include "script/compiler/signature.h"
#include "script/compiler/classresolver.h"

#include "variablecheckvisitor.h"

/*

ScopedTable :
- when entering a block add push scope
- leaving block pop scope

Member variables are at top level scope
When variable declared, they are added to the current scope.
*/

SymbolCheckVisitor::SymbolCheckVisitor(CompileContext& context):
   CompileStep(),
   mContext(context),
   mpClass(NULL),
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
      mContext.getLog().error("Function " + mpClass->getFullName() + "." + ast.getName() + " must have a return type (or equal class name as constructor).");
   }

   if ( ast.hasBody() )
   {
      if ( ast.getModifiers().isAbstract() )
      {
         mContext.getLog().error(String("Abstract function ") + mpClass->getName() + "." + ast.getName() + " should not have a body.");
      }
      else
      {
         checkReturn(ast);

         ast.getBody().accept(*this);
      }
   }
   else if ( !ast.getModifiers().isAbstract() && !ast.getModifiers().isNative() )
   {
      mContext.getLog().error(String("Function ") + mpClass->getName() + "." + ast.getName() + " requires a body.");
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
   if ( !mCurrentType.isBoolean() )
   {
      mContext.getLog().error(String("If condition expression must be of type boolean."));
   }

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

      if ( !mCurrentType.isBoolean() )
      {
         mContext.getLog().error(String("For loop condition expression must be of type boolean."));
      }
   }

   visitChildren(ast); // <-- loop expressions

   ast.getBody().accept(*this);
}

void SymbolCheckVisitor::visit(ASTForeach& ast)
{
   ScopedScope scope(mScopeStack);

   ASTVariable& var = ast.getVariable();
   if ( var.hasInit() )
   {
      ASTVariableInit& varinit = var.getInit();
      varinit.getExpression().accept(*this);

      const ASTClass& iterableclass = mContext.resolveClass("engine.collections.Iterable");

      if ( mCurrentType.isObject() && !mCurrentType.getObjectClass().isImplementing(iterableclass) )
      {
         mContext.getLog().error(String("Container ") + var.getName() + " must be iterable for use in foreach.");
      }
   }
   else
   {
      mContext.getLog().error(String("Compiler error: missing required initializer for foreach variable ") + var.getName());
   }

   ScopeVariable* pvariable = ScopeVariable::fromVariable(ast.getVariable());
   mScopeStack.add(pvariable);

   ast.getBody().accept(*this);
}

void SymbolCheckVisitor::visit(ASTWhile& ast)
{
   ast.getCondition().accept(*this);

   if ( !mCurrentType.isBoolean() )
   {
      mContext.getLog().error(String("While loop condition must be of type boolean."));
   }

   ast.getBody().accept(*this);
}

void SymbolCheckVisitor::visit(ASTDo& ast)
{
   ast.getBody().accept(*this);
   ast.getCondition().accept(*this);
   if ( !mCurrentType.isBoolean() )
   {
      mContext.getLog().error(String("Do loop condition must be of type boolean."));
   }
}

void SymbolCheckVisitor::visit(ASTSwitch& ast)
{
   ast.getExpression().accept(*this);
   ast.setType(mCurrentType.clone());

   if ( ast.getDefaultCount() > 1 )
   {
      mContext.getLog().error(String("A switch statement can have only one default case."));
   }

   visitChildren(ast);

   ast.validateCaseTypes(mContext);
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
         mContext.getLog().error(String("Function ") + mpFunction->getName() + " is a void function and can not return an object or value.");
      }
      else
      {
         ast.getExpression().accept(*this);

         if ( !mCurrentType.greater(mpFunction->getType()) )
         {
            mContext.getLog().error(String("Return type should be ") + mpFunction->getType().toString() + " but is " + mCurrentType.toString());
         }
      }
   }
   else if ( !mpFunction->getType().isVoid() )
   {
      mContext.getLog().error(String("Function ") + mpFunction->getName() + " should return a value of type " + mpFunction->getType().toString());
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
      ASTClass* pthrowable = mContext.findClass("system.Throwable");
      if ( pthrowable != NULL )
      {
         if ( !var.getType().getObjectClass().isBase(*pthrowable) )
         {
            ok = false;
         }
      }
      else
      {
         ok = false;
      }
   }

   if ( !ok )
   {
      mContext.getLog().error(String("Catch expects a throwable object."));
   }

   ast.getBody().accept(*this);
}

void SymbolCheckVisitor::visit(ASTThrow& ast)
{
   ast.getExpression().accept(*this);

   if ( mCurrentType.isObject() )
   {
      const ASTClass* pthrowable = mContext.findClass("system.Throwable");
      if ( !mCurrentType.getObjectClass().isBase(*pthrowable) )
      {
         mContext.getLog().error(String("Throw expression object must be derived from Throwable."));
      }
   }
   else
   {
      mContext.getLog().error(String("Throw requires an expression resulting in a throwable object."));
   }
}

void SymbolCheckVisitor::visit(ASTAssert& ast)
{
   mCurrentType.clear();

   ast.getCondition().accept(*this);

   if ( !mCurrentType.isBoolean() )
   {
      mContext.getLog().error(String("The assert condition expression must be of type boolean."));
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
            mContext.getLog().error(String("Invalid type for assignment. Can not assign ") + mCurrentType.toString() + " to " + lefttype.toString());
         }
      }
      else
      {
         mContext.getLog().error(String("Can only assign to variables."));
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
               String op("+");
               mContext.getLog().error(String("Can not execute operator ") + op + " on types " + lefttype.toString() + " and " + righttype.toString());
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
               mContext.getLog().error(String("Bitwise operators only operate on int values."));
            }
         }
         break;

      case ASTConcatenate::eAnd:
      case ASTConcatenate::eOr:
         {
            if ( !lefttype.isBoolean() || !mCurrentType.isBoolean() )
            {
               String op("&&"); // add toString to Mode
               mContext.getLog().error(String("Operator ") + op + " requires boolean expressions.");
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
               mContext.getLog().error(String("Can not compare type ") + lefttype.toString() + " with type " + mCurrentType.toString());
            }
            else if ( ast.getMode() >= ASTConcatenate::eSmallerEqual )
            {
               if ( comp.isObject() || comp.isArray() )
               {
                  mContext.getLog().error(String("Operator is not supported on arrays and objects."));
               }
               else if ( comp.isBoolean() )
               {
                  mContext.getLog().error(String("Operator is not supported on boolean."));
               }
            }

            mCurrentType = ASTType(ASTType::eBoolean);
         }
         break;
   }
}

void SymbolCheckVisitor::visit(ASTUnary& ast)
{
   visitChildren(ast);

   checkOperator(ast.getPre());
   checkOperator(ast.getPost());
}

void SymbolCheckVisitor::visit(ASTInstanceOf& ast)
{
   mCurrentType.clear();

   checkUnknown(ast.getInstanceType());

   ast.getObject().accept(*this);

   if ( !(mCurrentType.isObject() || mCurrentType.isArray()) )
   {
      mContext.getLog().error(String("Operator instanceof can only be called against objects/arrays."));
   }
   else if ( !mCurrentType.isDerivedFrom(ast.getInstanceType()) )
   {
      mContext.getLog().error(String("Instanceof operator can never be true for ") + mCurrentType.toString() + " and " + ast.getInstanceType().toString());
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

            Signature signature;
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
                  String arguments = String("(") + signature.toString() + ")";
                  mContext.getLog().error(String("No matching constructor ") + newclass.getFullName() + arguments + " defined.");
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
                  mContext.getLog().error(String("Array size expression should be of type int."));
               }
            }

            mCurrentType = ast.getType();
         }
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
   ASTClass* pclass = mpClass;
   if ( pclass->hasBaseType() && pclass->getBaseType().isUnknown() )
   {
      // skip calls to the unknown super class
      return;
   }

   if ( ast.getKind() == ASTSuper::eSuper )
      pclass = &mpClass->getBaseClass();

   if ( mpFunction->getModifiers().isStatic() )
   {
      String str = ast.getKind() == ASTSuper::eThis ? "this" : "super";
      mContext.getLog().error(String("Can not access ") + str + " from a static function.");
   }

   if ( ast.isCall() )
   {
      Signature signature;
      ASTNodes& arguments = ast.getArguments();
      for ( int index = 0; index < arguments.size(); index++ )
      {
         ASTExpression& expr = dynamic_cast<ASTExpression&>(arguments[index]);
         expr.accept(*this);

         signature.append(mCurrentType.clone());
      }

      // use the correct class for this super node
      ASTClass* psearchclass = ast.getKind() == ASTSuper::eThis ? mpClass : &mpClass->getBaseClass();

      ASTTypeList types;
      ASTFunction* pconstructor = pclass->findBestMatch(psearchclass->getName(), signature, types);
      if ( pconstructor != NULL )
      {
         ast.setConstructor(pconstructor);
      }
      else
      {
         String arguments = String("(") + signature.toString() + ")";
         mContext.getLog().error(String("No constructor ") + psearchclass->getFullName() + arguments + " defined.");
      }
   }
   else
   {
      mCurrentType.setKind(ASTType::eObject);
      mCurrentType.setObjectName(pclass->getName());
      mCurrentType.setObjectClass(*pclass);
   }
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
                  ast.setVariable(var);

                  const ASTType& type = var.getType();
                  if ( type.isGeneric() )
                  {
                     const ASTTypeVariable& typevariable = type.getTypeVariable();
                     mCurrentType = mCurrentType.getTypeArguments()[typevariable.getIndex()];
                  }
                  else
                  {
                     mCurrentType = type;
                  }
               }
               else
               {
                  mContext.getLog().error(String("Class ") + aclass.getName() + " has no member variable " + name);
               }
            }
            else
            {
               //
               // local variable
               const ScopeVariable* pvariable = mScopeStack.find(name);
               if ( pvariable != NULL )
               {
                  ast.setAccess(ASTAccess::eLocal);
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
                        mContext.getLog().error(String("Can not access instance member ") + var.getName());
                     }

                     // variable access on current class
                     ast.setAccess(ASTAccess::eField);
                     ast.setVariable(pfield->getVariable());

                     mCurrentType = pfield->getVariable().getType();
                  }
                  else
                  {
                     mContext.getLog().error(String("Identifier ") + name + " is not defined.");
                  }
               }
            }

            if ( wasstatic && !ast.getVariable().getModifiers().isStatic() )
            {
               mContext.getLog().error(String("Can not access non static variable ") + ast.getName() + " from static");
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
                        const ASTClass& arrayclass = mContext.resolveClass("system.InternalArray");
                        checkFunctionAccess(arrayclass, ast, wasstatic);
                     }
                     break;
                  case ASTType::eString:
                     {
                        const ASTClass& stringclass = mContext.resolveClass("system.InternalString");
                        checkFunctionAccess(stringclass, ast, wasstatic);
                     }
                     break;
                  case ASTType::eVoid:
                     {
                        mContext.getLog().error(String("Can not invoke a method on a void object."));
                     }
                     break;
                  default:
                     {
                        mContext.getLog().error(String("Can not invoke method on basic types."));
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
               mContext.getLog().error(mCurrentType.toString() + " is not an array type.");
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
                     mContext.getLog().error(String("Array access expression must be of type int."));
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
               mContext.getLog().warning(String("The class operator currently is only supported for objects."));
            }

            if ( !wasstatic )
               ast.setAccess(ASTAccess::eField);
            else
               ast.setAccess(ASTAccess::eRefField);

            mCurrentType.clear();
            mCurrentType.setKind(ASTType::eObject);
            mCurrentType.setObjectName("system.Class");
            mCurrentType.setObjectClass(mContext.resolveClass("system.Class"));
         }
         break;

      case ASTAccess::eInvalid:
      default:
         {
            mContext.getLog().error(String("Unknown access detected."));
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
         mContext.getLog().error(String("Assigning wrong type to variable '") + var.getName() + "' was expecting " + var.getType().toString() + " and got " + mCurrentType.toString());
      }
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
         mContext.getLog().error(String("Function ") + function.getName() + " should return a value of type " + function.getType().toString());
      }
      else if ( hasunreachablecode )
      {
         mContext.getLog().warning(String("Unreachable code in ") + function.getName());
      }
   }
}

void SymbolCheckVisitor::checkFunctionAccess(const ASTClass& aclass, ASTAccess& access, bool isstatic)
{
   ASTType before = mCurrentType;

   Signature signature;
   ASTNodes& arguments = access.getArguments();
   for ( int index = 0; index < arguments.size(); index++ )
   {
      ASTExpression& expr = dynamic_cast<ASTExpression&>(arguments[index]);
      expr.accept(*this);

      signature.append(mCurrentType.clone());
   }

   const ASTTypeList& typelist = access.getTypeArguments().size() > 0 ? access.getTypeArguments() : before.getTypeArguments();
   const ASTFunction* pfunction = aclass.findBestMatch(access.getName(), signature, typelist);

   if ( pfunction != NULL )
   {
      const Signature& funcsig = pfunction->getSignature();

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
         mContext.getLog().error(String("Can not call non static function ") + pfunction->getName());
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
         // here we should like the T of the original class with the T of the returning type
         const ASTType& arg = type.getTypeArguments()[0];
         const ASTTypeVariable* pvariable = aclass.getTypeVariables().find(arg.getObjectName());
         if ( pvariable != NULL )
         {
            mCurrentType = pfunction->getType();
            mCurrentType.replaceArgument(before.getTypeArguments()[pvariable->getIndex()]);
         }
      }
      else
      {
         mCurrentType = pfunction->getType();
      }
   }
   else
   {
      String arguments = String("(") + signature.toString() + ")";
      mContext.getLog().error(String("No matching function ") + aclass.getName() + "." + access.getName() + arguments + " defined.");
   }
}

void SymbolCheckVisitor::checkOperator(ASTUnary::Operator op)
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
               mContext.getLog().error(String("Pre operator requires a numeric value."));
            }
         }
         break;
      case ASTUnary::eNot:
         {
            if ( !mCurrentType.isBoolean() )
            {
               mContext.getLog().error(String("Not operator requires a boolean value."));
            }
         }
         break;
      }
}

void SymbolCheckVisitor::checkUnknown(const ASTType& type)
{
   if ( type.isUnknown() )
   {
      mContext.getLog().error(String("Unknown class type ") + type.getObjectName());
   }
}
