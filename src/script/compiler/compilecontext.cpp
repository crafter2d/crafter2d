
#include "compilecontext.h"

#include "core/string/string.h"
#include "core/conv/lexical.h"

#include "script/ast/astclass.h"
#include "script/ast/astvariable.h"

CompileContext::CompileContext():
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
   return findClass(classname) != NULL;
}

// - Operations

void CompileContext::addClass(ASTClass* pclass)
{
   String s = String(pclass->getName().c_str()).toLower();
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
   return mClasses.find(lowercasename) != mClasses.end() ? mClasses[lowercasename] : NULL;
}
