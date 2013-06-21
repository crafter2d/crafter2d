
#include "oocheckvisitor.h"

#include "core/defines.h"

#include "script/ast/ast.h"
#include "script/compiler/compilecontext.h"
#include "script/common/functionregistration.h"
#include "script/scope/scopevariable.h"
#include "script/scope/scopedscope.h"

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

   if ( mpClass->isClass() )
   {
      if ( mpClass->hasAbstractFunction() )
      {
         if ( !ast.getModifiers().isAbstract() )
         {
            mContext.getLog().error(String("Class ") + mpClass->getName() + " must be defined as abstract.");
         }
      }
      
      if ( ast.hasBaseClass() && ast.getBaseClass().getModifiers().isFinal() )
      {
         mContext.getLog().error(String("Class ") + ast.getName() + " can not extend final class " + ast.getBaseClass().getName());
      }
      
      if ( ast.getModifiers().isAbstract() && !mpClass->hasAbstractFunction() )
      {
         mContext.getLog().warning(String("Class ") + ast.getName() + " is marked abstract without abstract functions.");
      }
   }
   else 
   {
      ASSERT(mpClass->isInterface());
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
   mpFunction = &ast;

   ScopedScope scope(mScopeStack);

   ast.getArgumentNodes().accept(*this);

   if ( ast.hasAnnotations() )
   {
      if ( ast.getAnnotations().contains("override") )
      {
         // check if the base class still has this function
      }
   }

   if ( ast.isConstructor() )
   {
      // abstract classes can not have native constructors
      /*
      if ( ast.getModifiers().isNative() && mpClass->getModifiers().isAbstract() )
      {
         mContext.getLog().error("Abstract class " + mpClass->getFullName() + " can not have native constructors.");
      }
      */
   }
   else
   {
      if ( mpClass->hasBaseClass() )
      {
         ASTClass& baseclass = mpClass->getBaseClass();
         ASTFunction* pbasefunc = baseclass.findExactMatch(ast.getName(), ast.getSignature());
         if ( pbasefunc != NULL )
         {
            ast.setBaseFunction(*pbasefunc);
         }
      }
   }

   if ( ast.hasBody() )
   {
      mHasNativeCall = false;

      ast.getBody().accept(*this);

      if ( mHasNativeCall )
      {
         ast.getModifiers().setNative(true);
      }
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
         mContext.getLog().error((String("Can not assign a value to final variable ") + mpVariable->getName()).toStdString());
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

void OOCheckVisitor::visit(ASTNative& ast)
{
   mHasNativeCall = true;

   String name = mpFunction->getPrototype();

   const FunctionRegistration* preg = mContext.getClassRegistry().findCallback(*mpClass, name);
   if ( preg == NULL )
   {
      String qualitifiedname = mpClass->getFullName() + "_" + name;
      mContext.getLog().error("Native function " + qualitifiedname + " is not registered or has wrong arguments");
   }
   else
   {
      ast.setIndex(preg->getIndex());
   }
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
                  mContext.getLog().error(String("Method ") + mpCurrentType->getObjectClass().getName() + "." + function.getName() + " is not accessible.");
               }
            }
            else
            {
               if ( function.getModifiers().isPrivate() && !mpClass->isLocal(function) )
               {
                  mContext.getLog().error(String("Can not access private function ") + function.getName());
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
                  mContext.getLog().error(String("Can not access private variable ") + ast.getName());
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
                           mContext.getLog().error(String("Can not access private variable ") + ast.getName());
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

void OOCheckVisitor::validateClass(const ASTClass& klass)
{
   // check if all base class functions are implemented
   // does not have to be a direct class: abstract classes do not have to implement the abstract methods of their base
   // (abstract methods shouldnt be stored in the function table, but as they are its making life much easier)

   if ( !klass.getModifiers().isAbstract() && klass.hasBaseClass() )
   {
      
      /*
      const FunctionTable& functions = aclass.getFunctionTable();

      const ASTClass& baseclass = aclass.getBaseClass();
      const FunctionTable& basefunctions = baseclass.getFunctionTable();

      for ( int index = 0; index < basefunctions.size(); index++ )
      {
         const ASTFunction& function = functions[index];
         const ASTFunction& basefunction = basefunctions[index];

         if ( basefunction.getModifiers().isAbstract() && function.getModifiers().isAbstract() )
         {
            mContext.getLog().error(String("Function ") + aclass.getName() + "." + function.getName() + " must be implemented.");
         }
      }
      */
   }

    checkInterfaceImplementation(klass);
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
      mContext.getLog().error(String("Invalid concatenation with null operator! Only == and != are supported."));
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

void OOCheckVisitor::checkInterfaceImplementation(const ASTClass& ast)
{
   if ( !ast.getModifiers().isAbstract() )
   {
      ASTTypeList interfaces;
      ast.collectInterfaces(interfaces);

      for ( int index = 0; index < interfaces.size(); ++index )
      {
         ASTType& itype = interfaces[index];
         ASTClass& intrface = itype.getObjectClass();
         ASSERT(intrface.isInterface());

         ASTFunctionMap& functions = intrface.getFunctions();
         ASTFunctionMap::Iterator it = functions.getIterator();
         while ( functions.hasNext(it) )
         {
            ASTFunction& function = functions.getNext(it);
            const ASTFunction* pimplementation = ast.findExactMatch(function.getName(), function.getSignature());
            if ( pimplementation == NULL )
            {
               mContext.getLog().error("Function " + intrface.getFullName() + "." + function.getPrototype() + " is not implemented.");
            }
         }
      }
   }
}
