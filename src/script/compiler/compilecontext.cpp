
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

void CompileContext::addClass(ASTClass* pclass, const std::string& package)
{
   std::string full;
   if ( package.empty() )
      full = pclass->getName();
   else
      full = package + '/' + pclass->getName();

   String s = String(full.c_str()).toLower();
   std::string lowercasename = s.toStdString();
   
   mClasses[lowercasename] = pclass;
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

   Classes::iterator it = mClasses.find(lowercasename);
   if ( it == mClasses.end() )
   {
      std::string path = name;
      if ( lowercasename == "box2dbody" )
      {
         path = "box2d/" + name;
      }

      mCompiler.loadClass(path);
      it = mClasses.find(lowercasename);
   }

   return it != mClasses.end() ? it->second : NULL;
}
