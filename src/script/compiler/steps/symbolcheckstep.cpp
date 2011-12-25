
#include "symbolcheckstep.h"

#include "core/defines.h"

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

   mpFunction = &ast;

   visitChildren(ast); // <-- arguments

   if ( ast.hasBody() )
   {
      checkReturn(ast);

      ast.getBody().accept(*this);
   }

   // check if there was a return in the body

   mCurrentType.clear();
}

void SymbolCheckVisitor::visit(ASTFunctionArgument& ast)
{
   // check here for initialization expression (should be same type of the argument definition)

   ScopeVariable* pvariable = ScopeVariable::fromVariable(ast.getVariable());
   mScopeStack.add(pvariable);
}

void SymbolCheckVisitor::visit(ASTField& ast)
{
   ASTVariable& var = ast.getVariable();

   if ( var.hasExpression() )
   {
      var.getExpression().accept(*this);

      if ( !var.getType().equals(mCurrentType) )
      {
         mContext.getLog().error("Initialization expression is of wrong type. Expected " + var.getType().toString());
      }
   }
}

void SymbolCheckVisitor::visit(ASTBlock& ast)
{
   ScopedScope scope(mScopeStack);

   visitChildren(ast);
}

void SymbolCheckVisitor::visit(ASTLocalVariable& ast)
{
   ASTVariable& var = ast.getVariable();
   if ( var.hasExpression() )
   {
      var.getExpression().accept(*this);

      if ( !mCurrentType.greater(var.getType()) )
      {
         mContext.getLog().error("Assigning wrong type to variable " + var.getName() + " was expecting " + var.getType().toString() + " and got " + mCurrentType.toString());
      }
   }

   ScopeVariable* pvariable = ScopeVariable::fromVariable(ast.getVariable());
   mScopeStack.add(pvariable);
}

void SymbolCheckVisitor::visit(ASTExpressionStatement& ast)
{
   if ( ast.hasExpression() )
   {
      ast.getExpression().accept(*this);
   }
   else
   {
      mContext.getLog().warning("Expression statement without expression.");
   }
}

void SymbolCheckVisitor::visit(ASTIf& ast)
{
   ast.getCondition().accept(*this);
   if ( !mCurrentType.isBoolean() )
   {
      mContext.getLog().error("If condition expression must be of type boolean.");
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
         mContext.getLog().error("For loop condition expression must be of type boolean.");
      }
   }

   visitChildren(ast); // <-- loop expressions

   ast.getBody().accept(*this);
}

void SymbolCheckVisitor::visit(ASTForeach& ast)
{
   ScopedScope scope(mScopeStack);

   ASTVariable& var = ast.getVariable();
   if ( var.hasExpression() )
   {
      var.getExpression().accept(*this);

      const ASTClass& iterableclass = mContext.resolveClass("engine.collections.Iterable");

      if ( mCurrentType.isObject() && !mCurrentType.getObjectClass().isImplementing(iterableclass) )
      {
         mContext.getLog().error("Container must be iterable.");
      }
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
      mContext.getLog().error("While loop condition must be of type boolean.");
   }

   ast.getBody().accept(*this);
}

void SymbolCheckVisitor::visit(ASTDo& ast)
{
   ast.getBody().accept(*this);
   ast.getCondition().accept(*this);
   if ( !mCurrentType.isBoolean() )
   {
      mContext.getLog().error("Do loop condition must be of type boolean.");
   }
}

void SymbolCheckVisitor::visit(ASTSwitch& ast)
{
   ast.getExpression().accept(*this);
   ast.setType(mCurrentType.clone());
   
   if ( ast.getDefaultCount() > 1 )
   {
      mContext.getLog().error("Too many 'default' statements in switch statement (one is max).");
   }

   visitChildren(ast);

   ast.validateCaseTypes(mContext);
}

void SymbolCheckVisitor::visit(ASTCase& ast)
{
   mCurrentType.clear();

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

   ast.getBody().accept(*this);
}

void SymbolCheckVisitor::visit(ASTReturn& ast)
{
   if ( ast.hasExpression() )
   {
      if ( mpFunction->getType().isVoid() )
      {
         mContext.getLog().error("Function " + mpFunction->getName() + " is a void function and can not return an object or value.");
      }
      else
      {
         ast.getExpression().accept(*this);
      }
   }

   if ( !mCurrentType.greater(mpFunction->getType()) )
   {
      mContext.getLog().error("Return type should be " + mpFunction->getType().toString() + " but is " + mCurrentType.toString());
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
      ASTClass* pthrowable = mContext.findClass("System.Throwable");
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
      mContext.getLog().error("Catch expects a throwable object.");
   }

   ast.getBody().accept(*this);
}

void SymbolCheckVisitor::visit(ASTThrow& ast)
{
   ast.getExpression().accept(*this);

   if ( mCurrentType.isObject() )
   {
      const ASTClass* pthrowable = mContext.findClass("System.Throwable");
      if ( !mCurrentType.getObjectClass().isBase(*pthrowable) )
      {
         mContext.getLog().error("Throw expression object must be derived from Throwable.");
      }
   }
   else
   {
      mContext.getLog().error("Throw requires an expression resulting in a throwable object.");
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
            mContext.getLog().error("Invalid type for assignment. Can not assign " + mCurrentType.toString() + " to " + lefttype.toString());
         }
      }
      else
      {
         mContext.getLog().error("Can only assign to variables.");
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
               std::string op = "+";
               mContext.getLog().error("Can not execute operator " + op + " on types " + lefttype.toString() + " and " + righttype.toString());
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
               mContext.getLog().error("Bitwise operators only operate on int values.");
            }
         }
         break;

      case ASTConcatenate::eAnd:
      case ASTConcatenate::eOr:
         {
            if ( !lefttype.isBoolean() || !mCurrentType.isBoolean() )
            {
               std::string op = "&&"; // add toString to Mode
               mContext.getLog().error("Operator " + op + " requires boolean expressions.");
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
               mContext.getLog().error("Can not compare type " + lefttype.toString() + " with type " + mCurrentType.toString());
            }
            else if ( ast.getMode() >= ASTConcatenate::eSmallerEqual )
            {
               if ( comp.isObject() || comp.isArray() )
               {
                  mContext.getLog().error("Operator is not supported on arrays and objects.");
               }
               else if ( comp.isBoolean() )
               {
                  mContext.getLog().error("Operator is not supported on boolean.");
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
   ast.getObject().accept(*this);

   if ( !(mCurrentType.isObject() || mCurrentType.isArray()) )
   {
      mContext.getLog().error("Operator instanceof can only be called against objects/arrays.");
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

            const ASTClass& newclass = ast.getType().getObjectClass();
            const ASTFunction* pfunction = newclass.findBestMatch(newclass.getName(), signature, before.getTypeArguments());

            if ( pfunction == NULL )
            {
               std::string arguments = "(" + signature.toString() + ")";
               mContext.getLog().error("No matching constructor " + newclass.getFullName() + arguments + " defined.");
            }
            else
            {
               ast.setConstructor(*pfunction);
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
                  mContext.getLog().error("Array size expression should be of type int.");
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
   if ( ast.getKind() == ASTSuper::eSuper )
      pclass = &mpClass->getBaseClass();

   if ( mpFunction->getModifiers().isStatic() )
   {
      std::string str = ast.getKind() == ASTSuper::eThis ? "this" : "super";
      mContext.getLog().error("Can not access " + str + " from a static function.");
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
         std::string arguments = "(" + signature.toString() + ")";
         mContext.getLog().error("No constructor " + psearchclass->getFullName() + arguments + " defined.");
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
   const std::string& name = ast.getName();
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
                  mContext.getLog().error("Class " + aclass.getName() + " has no member variable " + name);
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

                     if ( mpFunction->getModifiers().isStatic() && !var.getModifiers().isStatic() )
                     {
                        mContext.getLog().error("Can not access instance member " + var.getName());
                     }

                     // variable access on current class
                     ast.setAccess(ASTAccess::eField);
                     ast.setVariable(pfield->getVariable());

                     mCurrentType = pfield->getVariable().getType();
                  }
                  else
                  {
                     mContext.getLog().error("Identifier " + name + " is not defined.");
                  }
               }
            }

            if ( wasstatic && !ast.getVariable().getModifiers().isStatic() )
            {
               mContext.getLog().error("Can not access non static variable " + ast.getName() + " from static");
            }
         }
         break;

      case ASTAccess::eFunction:
         {
            if ( mCurrentType.isValid() )
            {
               if ( !mCurrentType.isVoid() )
               {
                  if ( mCurrentType.isObject() )
                  {
                     ASSERT(mCurrentType.hasObjectClass());

                     const ASTClass& type = mCurrentType.getObjectClass();
                     checkFunction(type, ast, wasstatic);
                  }
                  else if ( mCurrentType.isArray() )
                  {
                     const ASTClass& arrayclass = mContext.resolveClass("System.InternalArray");
                     checkFunction(arrayclass, ast, wasstatic);
                  }
                  else
                  {
                     mContext.getLog().error("Can not invoke method on basic types.");
                  }
               }
               else
               {
                  mContext.getLog().error("Can not invoke a method on a void object.");
               }
            }
            else
            {
               checkFunction(*mpClass, ast, wasstatic);
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
               ASTType before = mCurrentType;

               ASTNodes& arguments = ast.getArguments();
               for ( int index = 0; index < arguments.size(); index++ )
               {
                  ASTExpression& expr = dynamic_cast<ASTExpression&>(arguments[index]);
                  expr.accept(*this);

                  if ( !mCurrentType.isInt() )
                  {
                     mContext.getLog().error("Array access expression must be of type int.");
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
               mContext.getLog().warning("The class operator currently is only supported for objects.");
            }

            if ( !wasstatic )
               ast.setAccess(ASTAccess::eField);
            else
               ast.setAccess(ASTAccess::eRefField);

            mCurrentType.clear();
            mCurrentType.setKind(ASTType::eObject);
            mCurrentType.setObjectName("System.Class");
            mCurrentType.setObjectClass(mContext.resolveClass("System.Class"));
         }
         break;

      case ASTAccess::eInvalid:
      default:
         {
            mContext.getLog().error("Unknown access detected.");
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

void SymbolCheckVisitor::checkReturn(const ASTFunction& function)
{
   if ( !function.getType().isVoid() )
   {
      // ensure that we have a return statement
      bool hasunreachablecode = false;
      if ( !function.getBody().hasReturn(hasunreachablecode) )
      {
         mContext.getLog().error("Function " + function.getName() + " should return a value of type " + function.getType().toString());
      }
      else if ( hasunreachablecode )
      {
         mContext.getLog().warning("Unreachable code in " + function.getName());
      }
   }
}

void SymbolCheckVisitor::checkFunction(const ASTClass& aclass, ASTAccess& access, bool isstatic)
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

   const ASTFunction* pfunction = aclass.findBestMatch(access.getName(), signature, before.getTypeArguments());

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
         mContext.getLog().error("Can not call non static function " + pfunction->getName());
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
      std::string arguments = "(" + signature.toString() + ")";
      mContext.getLog().error("No matching function " + aclass.getName() + "." + access.getName() + arguments + " defined.");
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
               mContext.getLog().error("Pre operator requires a numeric value.");
            }
         }
         break;
      case ASTUnary::eNot:
         {
            if ( !mCurrentType.isBoolean() )
            {
               mContext.getLog().error("Not operator requires a boolean value.");
            }
         }
         break;
      }
}