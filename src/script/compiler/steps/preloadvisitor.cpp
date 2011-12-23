
#include "preloadvisitor.h"

#include "script/compiler/compiler.h"
#include "script/compiler/exceptions/classnotfoundexception.h"

#include "script/ast/ast.h"
#include "script/scope/scopedscope.h"
#include "script/scope/scopevariable.h"

PreloadVisitor::PreloadVisitor(CompileContext& context):
   CompileStep(),
   mContext(context),
   mClassResolver(),
   mPackage(),
   mScopeStack(),
   mpClass(NULL)
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

   mClassResolver.insert(mPackage + ".*");
}

void PreloadVisitor::visit(ASTUse& ast)
{
   mClassResolver.insert(ast.getIdentifier());
}

void PreloadVisitor::visit(ASTClass& ast)
{
   ScopedScope scope(mScopeStack);

   mpClass = &ast;
   mContext.addClass(&ast);

   if ( ast.hasBaseType() )
   {
      if ( load(ast.getBaseType()) )
      {
         ast.setBaseClass(mContext.resolveClass(ast.getBaseType().getObjectName()));
      }
      else
      {
         return;
      }
   }
   
   ASTTypeList& intrfaces = ast.getInterfaces();
   for ( int index = 0; index < intrfaces.size(); index++ )
   {
      ASTType& type = intrfaces[index];
      if ( !load(type) )
      {
         // could not load type
      }
   }
   
   visitChildren(ast);

   ast.setState(ASTClass::eLoaded);
}

void PreloadVisitor::visit(ASTFunction& ast)
{
   ScopedScope scope(mScopeStack);

   if ( !load(ast.getType()) )
   {
      // type is not known :(
   }

   visitChildren(ast); // <-- arguments

   if ( ast.hasBody() )
   {
      ast.getBody().accept(*this);
   }
}

void PreloadVisitor::visit(ASTFunctionArgument& ast)
{
   ASTVariable& var = ast.getVariable();

   if ( !load(var.getType()) ) // <-- need to give class as param so the previous typevariable stuff can be moved there
   {
      // complain!
   }

   if ( var.hasExpression() )
   {
      var.getExpression().accept(*this);
   }

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

   if ( var.hasExpression() )
   {
      var.getExpression().accept(*this);
   }
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

   if ( var.hasExpression() )
   {
      var.getExpression().accept(*this);
   }

   ScopeVariable* pvariable = ScopeVariable::fromVariable(var);
   mScopeStack.add(pvariable);
}

void PreloadVisitor::visit(ASTExpressionStatement& ast)
{
   if ( ast.hasExpression() )
   {
      ast.getExpression().accept(*this);
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

   if ( var.hasExpression() )
   {
      var.getExpression().accept(*this);
   }

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
      const std::string& name = type.getObjectName();

      /*
      // check the type arguments
      const ASTTypeList& typearguments = type.getTypeArguments();
      for ( int index = 0; index < typearguments.size(); index++ )
      {
         if ( !mTypeArguments[index].resolveType(context, aclass) )
         {
            return false;
         }
      }
      */
      if ( type.getObjectName() == "T" )
      {
         return true;
      }

      if ( !tryLoad(type) )
      {
         throw new ClassNotFoundException(type.getObjectName());
      }
   }

   return true;
}

bool PreloadVisitor::tryLoad(ASTType& type)
{
   return mContext.loadClass(type.getObjectName());
}

// - Operations

void PreloadVisitor::checkStaticAccess(ASTUnary& unary)
{
   ASTAccess* paccess = dynamic_cast<ASTAccess*>(&unary.getParts()[0]);
   if ( paccess != NULL )
   {
      ASTField* pfield = mpClass->findField(paccess->getName());
      if ( pfield != NULL )
      {
         // it's a member field, so nothing to do here
         return;
      }

      ScopeVariable* pvariable = mScopeStack.find(paccess->getName());
      if ( pvariable == NULL )
      {
         std::string name;
         std::string qualifiedname;
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
               std::size_t pos = qualifiedname.rfind('.');
               if ( pos != std::string::npos )
               {
                  std::string path = qualifiedname.substr(0, pos) + ".*";
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