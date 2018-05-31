
#include "oocheckvisitor.h"

#include "core/defines.h"

#include "yasc/compiler/ast/ast.h"
#include "yasc/compiler/errornumbers.h"
#include "yasc/compiler/compilecontext.h"
#include "yasc/compiler/scope/scopevariable.h"
#include "yasc/compiler/scope/scopedscope.h"

OOCheckVisitor::OOCheckVisitor(CompileContext& context):
   CompileStep(context),
   mpVariable(nullptr),
   mpCurrentType(nullptr),
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
            error(E0052, UTEXT("Class ") + mpClass->getName() + UTEXT(" must be defined as abstract."), ast);
         }
      }
      
      if ( ast.hasBaseClass() && ast.getBaseClass().getModifiers().isFinal() )
      {
         error(E0053, UTEXT("Class ") + ast.getName() + UTEXT(" can not extend final class ") + ast.getBaseClass().getName(), ast);
      }
      
      if ( ast.getModifiers().isAbstract() && !mpClass->hasAbstractFunction() )
      {
         warning(W0004, UTEXT("Class ") + ast.getName() + UTEXT(" is marked abstract without abstract functions."), ast);
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
      if ( ast.getAnnotations().contains(UTEXT("override")) )
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
         if ( pbasefunc != nullptr )
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
   mpCurrentType = nullptr;

   ast.getLeft().accept(*this);

   if ( ast.hasRight() )
   {
      // ensure the variable is not final
      if ( mpVariable != nullptr && mpVariable->getModifiers().isFinal() )
      {
         error(E0054, UTEXT("Can not assign a value to final variable ") + mpVariable->getName(), ast);
      }

      mpCurrentType = nullptr;

      ast.getRight().accept(*this);
   }
}

void OOCheckVisitor::visit(ASTConcatenate& ast)
{
   mpCurrentType = nullptr;

   ast.getLeft().accept(*this);
   if ( ast.hasRight() )
   {
      const ASTType* plefttype = mpCurrentType;

      mpCurrentType = nullptr;

      ast.getRight().accept(*this);

      if ( plefttype != nullptr && mpCurrentType != nullptr )
      {
         validatenullptrConcatenate(ast, *plefttype, *mpCurrentType);
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
   C2D_UNUSED(ast);
}

void OOCheckVisitor::visit(ASTNative& ast)
{
   C2D_UNUSED(ast);
   
   mHasNativeCall = true;
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
            if ( mpCurrentType != nullptr )
            {
               if ( !function.getModifiers().isPublic() )
               {
                  error(E0055, UTEXT("Method ") + mpCurrentType->getObjectClass().getName() + '.' + function.getName() + UTEXT(" is not accessible."), ast);
               }
            }
            else
            {
               if ( function.getModifiers().isPrivate() && !mpClass->isLocal(function) )
               {
                  error(E0056, UTEXT("Can not access private function ") + function.getName(), ast);
               }
            }
         }
         break;
      case ASTAccess::eVariable:
         {
            if ( mpCurrentType != nullptr )
            {
               ASSERT(mpCurrentType->isObject() || mpCurrentType->isArray());

               const ASTVariable& var = ast.getVariable();
               checkPublicAccess(mpCurrentType->getObjectClass(), var);

               mpVariable = &var;
               mpCurrentType = &var.getType();
            }
            else
            {
               const ASTField* pfield = mpClass->findField(ast.getName(), ASTClass::eLocal);
               if ( pfield == nullptr )
               {
                  if ( mpClass->hasBaseClass() )
                  {
                     pfield = mpClass->getBaseClass().findField(ast.getName());
                     if ( pfield != nullptr )
                     {
                        checkPublicAccess(*mpClass, pfield->getVariable());

                        mpVariable = &pfield->getVariable();
                        mpCurrentType = &mpVariable->getType();
                     }
                  }
                  else
                  {
                     // ScopeVariable* pvariable = mScopeStack.find(ast.getName());
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
         
      default:
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
   C2D_UNUSED(expr);
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

void OOCheckVisitor::validatenullptrConcatenate(ASTConcatenate& concatenate, const ASTType& left, const ASTType& right)
{
   bool haserror = false;

   if ( left.isnullptr() )
   {
      haserror = ( !right.isObject() && !right.isArray() );

      if ( concatenate.getMode() == ASTConcatenate::eEquals || concatenate.getMode() == ASTConcatenate::eUnequals )
      {
         // swap left/right side so nullptr is always on righthand side (easier for code generation)
         concatenate.swapSides();
      }
   }
   else if ( right.isnullptr() )
   {
      haserror = ( !left.isObject() && !left.isArray() );
   }

   if ( haserror )
   {
      error(E0058, UTEXT("Invalid concatenation with nullptr operator! Only == and != are supported."), concatenate);
   }
}

void OOCheckVisitor::checkPublicAccess(const ASTClass& klass, const ASTVariable& var)
{
   if ( var.getModifiers().isPrivate() )
   {
      error(E0057, UTEXT("Can not access private variable ") + klass.getFullName() + L'.' + var.getName(),var.getPosition());
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
            if ( pimplementation == nullptr )
            {
               error(E0058, UTEXT("Function ") + intrface.getFullName() + '.' + function.getPrototype() + UTEXT(" is not implemented."), ast);
            }
         }
      }
   }
}
