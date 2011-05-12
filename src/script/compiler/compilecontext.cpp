
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
   mpResult(NULL),
   mCollect(true)
{
   mResolver.insert("System.*");
   mResolver.insert(".*");
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

void CompileContext::resetCollection()
{
   mCollect = true;
}

// - Query

bool CompileContext::hasClass(const std::string& classname) const
{
   std::string fullname = mResolver.resolve(classname);
   if ( fullname.empty() )
      return NULL;

   String s = String(fullname.c_str()).toLower();
   std::string lowercasename = s.toStdString();

   return mClasses.find(lowercasename) != mClasses.end();
}

std::string CompileContext::getFullName(const std::string& classname) const
{
   if ( classname.find('.') == std::string::npos )
      return mResolver.resolve(classname);
   else
      return classname;
}

// - Operations

void CompileContext::addClass(ASTClass* pclass)
{
   String s = String(pclass->getFullName().c_str()).toLower();
   std::string lowercasename = s.toStdString();
   
   mClasses[lowercasename] = pclass;
   mCollect = false;
}

void CompileContext::addPath(const std::string& path)
{
   if ( mCollect )
   {
      mResolver.insert(path);
   }
}

// - Search

const ASTClass* CompileContext::findClass(const std::string& classname) const
{
   return const_cast<CompileContext*>(this)->findClass(classname);
}

ASTClass* CompileContext::findClass(const std::string& name)
{
   std::string fullname = mResolver.resolve(name);
   if ( fullname.empty() )
      return NULL;

   String s = String(fullname.c_str()).toLower();
   std::string lowercasename = s.toStdString();

   Classes::iterator it = mClasses.find(lowercasename);
   if ( it == mClasses.end() )
   {
      if ( mCompiler.loadClass(fullname) )
      {
         it = mClasses.find(lowercasename);
      }
   }

   return it != mClasses.end() ? it->second : NULL;
}
