
#include "compilecontext.h"

#include <algorithm>

#include "core/defines.h"
#include "core/string/string.h"
#include "core/conv/lexical.h"

#include "ast/astclass.h"
#include "ast/astroot.h"
#include "ast/astvariable.h"
#include "ast/asttype.h"

#include "steps/preloadvisitor.h"

#include "exceptions/classnotfoundexception.h"
#include "exceptions/filenotfoundexception.h"

#include "compiler.h" 

CompileContext::CompileContext(Compiler& compiler):
   mCompiler(compiler),
   mClasses(),
   mLiteralTable(),
   mLog()
{
}

CompileContext::~CompileContext()
{
}

// - Get/set

CompileLog& CompileContext::getLog()
{
   return mLog;
}

const LiteralTable& CompileContext::getLiteralTable() const
{
   return mLiteralTable;
}
         
LiteralTable& CompileContext::getLiteralTable()
{
   return mLiteralTable;
}

Compiler& CompileContext::getCompiler()
{
   return mCompiler;
}

// - Query

bool CompileContext::hasClass(const String& classname) const
{
   String lowercasename = classname;
   lowercasename.toLower();

   return mClasses.find(lowercasename) != mClasses.end();
}

// - Operations

void CompileContext::addClass(ASTClass* pclass)
{
   String lowercasename = pclass->getFullName();
   lowercasename.toLower();
   
   mClasses[lowercasename] = pclass;
}

void CompileContext::addInterface(ASTClass& interfce)
{
   mInterfaces.add(interfce);
}

bool CompileContext::loadClass(const String& classname)
{
   String lowercasename = classname;
   lowercasename.toLower();

   ClassMap::iterator it = mClasses.find(lowercasename);
   if ( it == mClasses.end() )
   {
      try
      {
         ASTRoot& root = mCompiler.load(classname);
         std::vector<ASTClass*> classes;
         root.collectClasses(classes);
         auto append = [this](ASTClass* pclass) { addClass(pclass); };
         std::for_each(classes.begin(), classes.end(), append);
      }
      catch ( FileNotFoundException* )
      {
         return false;
      }
   }

   return true;
}

void CompileContext::loadBaseAndInterfaces(ASTClass* pclass)
{
   if ( pclass->hasBaseType() )
   {
      loadClass(pclass->getBaseType().getObjectName());
      pclass->setBaseClass(resolveClass(pclass->getBaseType().getObjectName()));
   }

   ASTTypeList& intrfaces = pclass->getInterfaces();
   for ( int index = 0; index < intrfaces.size(); index++ )
   {
      ASTType& type = intrfaces[index];
      loadClass(type.getObjectName());
      type.setObjectClass(resolveClass(type.getObjectName()));
   }
}

// - Search

const ASTClass* CompileContext::findClass(const String& classname) const
{
   return const_cast<CompileContext*>(this)->findClass(classname);
}

ASTClass* CompileContext::findClass(const String& name)
{
   String lowercasename(name);
   lowercasename.toLower();

   ClassMap::iterator it = mClasses.find(lowercasename);
   return it != mClasses.end() ? it->second : nullptr;
}

const ASTClass& CompileContext::resolveClass(const String& classname) const
{
   return const_cast<CompileContext*>(this)->resolveClass(classname);
}

ASTClass& CompileContext::resolveClass(const String& classname)
{
   ASTClass* pclass = findClass(classname);
   if ( pclass == nullptr )
   {
      throw new ClassNotFoundException(classname);
   }
   return *pclass;
}
