
#include "compilecontext.h"

#include <boost/algorithm/string.hpp>

#include "ast/astclass.h"
#include "ast/astvariable.h"

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
   std::string name = pclass->getName();
   boost::to_lower(name);
   mClasses[name] = pclass;
}

// - Search

const ASTClass* CompileContext::findClass(const std::string& classname) const
{
   return const_cast<CompileContext*>(this)->findClass(classname);
}

ASTClass* CompileContext::findClass(const std::string& name)
{
   std::string lowercasename = name;
   boost::to_lower(lowercasename);
   return mClasses.find(lowercasename) != mClasses.end() ? mClasses[lowercasename] : NULL;
}
