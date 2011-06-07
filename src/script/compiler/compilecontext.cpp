
#include "compilecontext.h"

#include "core/defines.h"
#include "core/string/string.h"
#include "core/conv/lexical.h"

#include "script/ast/astclass.h"
#include "script/ast/astvariable.h"

#include "compiler.h" 

CompileContext::CompileContext(Compiler& compiler):
   mCompiler(compiler),
   mClasses(),
   mLiteralTable(),
   mLog(),
   mpResult(NULL)
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

VirtualClass* CompileContext::getResult()
{
   return mpResult;
}

void CompileContext::setResult(VirtualClass* pclass)
{
   mpResult = pclass;
}

// - Query

bool CompileContext::hasClass(const std::string& classname) const
{
   String s = String(classname.c_str()).toLower();
   std::string lowercasename = s.toStdString();

   return mClasses.find(lowercasename) != mClasses.end();
}

// - Operations

void CompileContext::addClass(ASTClass* pclass)
{
   String s = String(pclass->getFullName().c_str()).toLower();
   std::string lowercasename = s.toStdString();
   
   mClasses[lowercasename] = pclass;
}

bool CompileContext::loadClass(const std::string& classname)
{
   String s = String(classname.c_str()).toLower();
   std::string lowercasename = s.toStdString();

   ClassMap::iterator it = mClasses.find(lowercasename);
   if ( it == mClasses.end() )
   {
      return mCompiler.load(classname);
   }

   return true;
}

void CompileContext::collectCompileClasses(std::vector<ASTClass*>& classes)
{
   ClassMap::iterator it = mClasses.begin();
   for ( ; it != mClasses.end(); it++ )
   {
      ASTClass* pclass = it->second;
      if ( pclass->getState() == ASTClass::eLoaded )
      {
         classes.push_back(pclass);
      }
   }
}

// - Search

const ASTClass* CompileContext::findClass(const std::string& classname) const
{
   return const_cast<CompileContext*>(this)->findClass(classname);
}

ASTClass* CompileContext::findClass(const std::string& name)
{
   String s = String(name.c_str()).toLower();
   std::string lowercasename = s.toStdString();

   ClassMap::iterator it = mClasses.find(lowercasename);
   return it != mClasses.end() ? it->second : NULL;
}

const ASTClass& CompileContext::resolveClass(const std::string& classname) const
{
   return const_cast<CompileContext*>(this)->resolveClass(classname);
}

ASTClass& CompileContext::resolveClass(const std::string& classname)
{
   ASTClass* pclass = findClass(classname);
   if ( pclass == NULL )
   {
      throw new std::exception();
   }
   return *pclass;
}
