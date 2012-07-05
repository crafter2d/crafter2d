
#include "oocheckvisitor.h"

#include "core/defines.h"

#include "script/ast/ast.h"
#include "script/compiler/compilecontext.h"
#include "script/compiler/signature.h"
#include "script/scope/scopevariable.h"
#include "script/scope/scopedscope.h"

#include "variablecheckvisitor.h"

OOCheckVisitor::OOCheckVisitor(CompileContext& context):
   CompileStep(),
   mContext(context),
   mpClass(NULL),
   mpVariable(NULL),
   mpCurrentType(NULL),
   mScopeStack()
{
}

// - Overloads

bool OOCheckVisitor::performStep(ASTNode& node)
{
   node.accept(*this);
   return true;
}

// - Visitor

void OOCheckVisitor::visit(ASTRoot& ast)
{
   visitChildren(ast);
}

void OOCheckVisitor::visit(ASTClass& ast)
{
   mpClass = &ast;
   ASSERT_PTR(mpClass);

   if ( mpClass->getKind() == ASTClass::eClass )
   {
      if ( mpClass->hasAbstractFunction() )
      {
         if ( !ast.getModifiers().isAbstract() )
         {
            mContext.getLog().error("Class " + mpClass->getName() + " must be abstract.");
         }
      }
      
      if ( ast.hasBaseClass() && ast.getBaseClass().getModifiers().isFinal() )
      {
         mContext.getLog().error("Class "+ ast.getName() + " can not extend final class " + ast.getBaseClass().getName());
      }
      
      if ( ast.getModifiers().isAbstract() )
      {
         mContext.getLog().warning("Class " + ast.getName() + " is marked abstract without abstract functions.");
      }
   }

   if ( ast.isNative() )
   {
      // a class is native if its either marked native or has one or native functions
      ast.getModifiers().setNative();
   }

   validateClass(ast);
   
   visitChildren(ast);
}

void OOCheckVisitor::visit(ASTFunction& ast)
{
   ScopedScope scope(mScopeStack);

   ast.getArguments().accept(*this);

   if ( ast.hasAnnotations() )
   {
      if ( ast.getAnnotations().contains("override") )
      {
         // check if the base class still has this function
      }
   }

   if ( ast.isConstructor() )
   {
      if ( !hasSuperCall(ast) )
      {
         std::string constructor = mpClass->getFullName() + "." + ast.getName() + "(" + ast.getSignature().toString() + ")";
         mContext.getLog().error("Constructor " + constructor + " must call super.");
      }
   }
   
   /*
   if ( ast.getModifiers().isNative() )
   {
      if ( ast.isConstructor() )
      {
         // mark class native when a it contains a native constructor, it should also not have a native base class
         if ( mpClass->hasBaseClass() && mpClass->getBaseClass().getModifiers().isNative() )
         {
            mContext.getLog().error("Native class + " + mpClass->getName() + " can not have a native base class " + mpClass->getBaseClass().getName() + ".");
         }

         mpClass->getModifiers().setNative();
      }
   }
   */

   if ( ast.hasBody() )
   {
      ast.getBody().accept(*this);
   }
}

void OOCheckVisitor::visit(ASTFunctionArgument& ast)
{
   ASTVariable& var = ast.getVariable();

   ScopeVariable* pvariable = ScopeVariable::fromVariable(var);
   mScopeStack.add(pvariable);

   mpVariable = &var;
   
   checkVarInit(var);
}

void OOCheckVisitor::visit(ASTField& ast)
{
   ASTVariable& var = ast.getVariable();

   mpVariable = &var;
   
   checkVarInit(var);
}

void OOCheckVisitor::visit(ASTLocalVariable& ast)
{
   ASTVariable& var = ast.getVariable();

   ScopeVariable* pvariable = ScopeVariable::fromVariable(var);
   mScopeStack.add(pvariable);

   mpVariable = &var;
   
   checkVarInit(var);
}

void OOCheckVisitor::visit(ASTBlock& ast)
{
   ScopedScope scope(mScopeStack);

   visitChildren(ast);
}

void OOCheckVisitor::visit(ASTExpressionStatement& ast)
{
   if ( ast.hasExpression() )
   {
      ast.getExpression().accept(*this);
   }
}

void OOCheckVisitor::visit(ASTIf& ast)
{
   ast.getCondition().accept(*this);
   ast.getStatement().accept(*this);

   if ( ast.hasElseStatement() )
   {
      ast.getElseStatement().accept(*this);
   }
}

void OOCheckVisitor::visit(ASTFor& ast)
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

   visitChildren(ast);
}

void OOCheckVisitor::visit(ASTForeach& ast)
{
   ASTVariable& var = ast.getVariable();

   ScopeVariable* pvariable = ScopeVariable::fromVariable(var);
   mScopeStack.add(pvariable);
   
   checkVarInit(var);

   ast.getBody().accept(*this);
}

void OOCheckVisitor::visit(ASTWhile& ast)
{
   ast.getCondition().accept(*this);
   ast.getBody().accept(*this);
}

void OOCheckVisitor::visit(ASTDo& ast)
{
   ast.getBody().accept(*this);
   ast.getCondition().accept(*this);
}

void OOCheckVisitor::visit(ASTSwitch& ast)
{
   ast.getExpression().accept(*this);

   visitChildren(ast);
}

void OOCheckVisitor::visit(ASTCase& ast)
{
   ast.getBody().accept(*this);
}

void OOCheckVisitor::visit(ASTReturn& ast)
{
   if ( ast.hasExpression() )
   {
      ast.getExpression().accept(*this);
   }
}

void  OOCheckVisitor::visit(ASTAssert& ast)
{
   ast.getCondition().accept(*this);
}

void OOCheckVisitor::visit(ASTExpression& ast)
{
   mpCurrentType = NULL;

   ast.getLeft().accept(*this);

   if ( ast.hasRight() )
   {
      // ensure the variable is not final
      if ( mpVariable != NULL && mpVariable->getModifiers().isFinal() )
      {
         mContext.getLog().error("Can not assign a value to final variable " + mpVariable->getName());
      }

      mpCurrentType = NULL;

      ast.getRight().accept(*this);
   }
}

void OOCheckVisitor::visit(ASTConcatenate& ast)
{
   mpCurrentType = NULL;

   ast.getLeft().accept(*this);
   if ( ast.hasRight() )
   {
      const ASTType* plefttype = mpCurrentType;

      mpCurrentType = NULL;

      ast.getRight().accept(*this);

      if ( plefttype != NULL && mpCurrentType != NULL )
      {
         validateNullConcatenate(ast, *plefttype, *mpCurrentType);
      }
   }
}

void OOCheckVisitor::visit(ASTUnary& ast)
{
   visitChildren(ast);
}

void OOCheckVisitor::visit(ASTNew& ast)
{
   visitChildren(ast);
}

void OOCheckVisitor::visit(ASTSuper& ast)
{
}

void OOCheckVisitor::visit(ASTAccess& ast)
{
   switch ( ast.getKind() )
   {
      case ASTAccess::eArray:
         {
         }
         break;
      case ASTAccess::eFunction:
         {
            const ASTFunction& function = ast.getFunction();
            if ( mpCurrentType != NULL )
            {
               if ( !function.getModifiers().isPublic() )
               {
                  mContext.getLog().error("Method " + mpCurrentType->getObjectClass().getName() + "." + function.getName() + " is not accessible.");
               }
            }
            else
            {
               if ( function.getModifiers().isPrivate() && !mpClass->isLocal(function) )
               {
                  mContext.getLog().error("Can not access private function " + function.getName());
               }
            }
         }
         break;
      case ASTAccess::eVariable:
         {
            if ( mpCurrentType != NULL )
            {
               ASSERT(mpCurrentType->isObject() || mpCurrentType->isArray());

               const ASTVariable& var = ast.getVariable();

               if ( !var.getModifiers().isPublic() )
               {
                  mContext.getLog().error("Can not access private variable " + ast.getName());
               }

               mpVariable = &var;
               mpCurrentType = &var.getType();
            }
            else
            {
               const ASTField* pfield = mpClass->findField(ast.getName(), ASTClass::eLocal);
               if ( pfield == NULL )
               {
                  if ( mpClass->hasBaseClass() )
                  {
                     pfield = mpClass->getBaseClass().findField(ast.getName());
                     if ( pfield != NULL )
                     {
                        if ( pfield->getVariable().getModifiers().isPrivate() )
                        {
                           mContext.getLog().error("Can not access private variable " + ast.getName());
                        }

                        mpVariable = &pfield->getVariable();
                        mpCurrentType = &mpVariable->getType();
                     }
                  }
                  else
                  {
                     ScopeVariable* pvariable = mScopeStack.find(ast.getName());
                  }
               }
               else
               {
                  // trying to access a local class member variables
                  mpVariable = &pfield->getVariable();
                  mpCurrentType = &mpVariable->getType();
               }
            }
         }
         break;

      case ASTAccess::eStatic:
         {
            mpCurrentType = &ast.getStaticType();
         }
         break;
   }
}

void OOCheckVisitor::visit(ASTCompound& compound)
{
   compound.getExpression().accept(*this);
}

// - Operations

bool OOCheckVisitor::isFinal(ASTNode& expr)
{
   return false;
}

bool OOCheckVisitor::hasSuperCall(const ASTFunction& function) const
{
   if ( function.hasBody() && mpClass->hasBaseClass() )
   {
      // ensure that there is a call to super
      const ASTBlock& block = function.getBody();
      if ( block.hasChildren() )
      {
         const ASTStatement& statement = function.getBody().getStatement(0);
         const ASTExpressionStatement* pexprstmt = dynamic_cast<const ASTExpressionStatement*>(&statement);
         if ( pexprstmt != NULL )
         {
            const ASTUnary* punary = dynamic_cast<const ASTUnary*>(&pexprstmt->getExpression().getLeft());
            if ( punary != NULL )
            {
               ASSERT(!punary->getParts().isEmpty());
               const ASTExpressionPart* ppart = dynamic_cast<const ASTExpressionPart*>(&punary->getParts()[0]);
               if ( ppart != NULL )
               {
                  return dynamic_cast<const ASTSuper*>(ppart) != NULL;
               }
            }
         }
      }

      return false;
   }
   return true;
}

void OOCheckVisitor::validateClass(const ASTClass& aclass)
{
   // check if all base class functions are implemented
   // does not have to be a direct class: abstract classes do not have to implement the abstract methods of their base
   // (abstract methods shouldnt be stored in the function table, but as they are its making life much easier)

   if ( !aclass.getModifiers().isAbstract() && aclass.hasBaseClass() )
   {
      const FunctionTable& functions = aclass.getFunctionTable();

      const ASTClass& baseclass = aclass.getBaseClass();
      const FunctionTable& basefunctions = baseclass.getFunctionTable();

      for ( int index = 0; index < basefunctions.size(); index++ )
      {
         const ASTFunction& function = functions[index];
         const ASTFunction& basefunction = basefunctions[index];

         if ( basefunction.getModifiers().isAbstract() && function.getModifiers().isAbstract() )
         {
            mContext.getLog().error("Function " + aclass.getName() + "." + function.getName() + " must be implemented.");
         }
      }
   }
}

void OOCheckVisitor::validateNullConcatenate(ASTConcatenate& concatenate, const ASTType& left, const ASTType& right)
{
   bool error = false;

   if ( left.isNull() )
   {
      error = ( !right.isObject() && !right.isArray() );

      if ( concatenate.getMode() == ASTConcatenate::eEquals || concatenate.getMode() == ASTConcatenate::eUnequals )
      {
         // swap left/right side so null is always on righthand side (easier for code generation)
         concatenate.swapSides();
      }
   }
   else if ( right.isNull() )
   {
      error = ( !left.isObject() && !left.isArray() );
   }

   if ( error )
   {
      mContext.getLog().error("Invalid concatenation with null operator! Only == and != are supported.");
   }
}

void OOCheckVisitor::checkVarInit(ASTVariable& var)
{
   if ( var.hasInit() )
   {
      ASTVariableInit& varinit = var.getInit();

      if ( varinit.hasArrayInit() )
      {
         varinit.getArrayInit().accept(*this);
      }
      else if ( varinit.hasExpression() )
      {
         varinit.getExpression().accept(*this);
      }
   }
}
