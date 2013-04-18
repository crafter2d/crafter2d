
#include "compilecontext.h"

#include "core/defines.h"
#include "core/string/string.h"
#include "core/conv/lexical.h"

#include "script/ast/astclass.h"
#include "script/ast/astvariable.h"

#include "exceptions/classnotfoundexception.h"

#include "compiler.h" 

CompileContext::CompileContext(Compiler& compiler):
   mCompiler(compiler),
   mClasses(),
   mClassRegistry(),
   mpByteCodeGenerator(NULL),
   mStringCache(),
   mLiteralTable(),
   mLog()
{
}

// - Get/set

CompileLog& CompileContext::getLog()
{
   return mLog;
}

StringCache& CompileContext::getStringCache()
{
   return mStringCache;
}

const LiteralTable& CompileContext::getLiteralTable() const
{
   return mLiteralTable;
}
         
LiteralTable& CompileContext::getLiteralTable()
{
   return mLiteralTable;
}

const ClassRegistry& CompileContext::getClassRegistry() const
{
   return mClassRegistry;
}

void CompileContext::setClassRegistry(const ClassRegistry& registry)
{
   mClassRegistry.add(registry);
}

CodeGen::IRGenerator& CompileContext::getByteCodeGenerator()
{
   ASSERT_PTR(mpByteCodeGenerator);
   return *mpByteCodeGenerator;
}

void CompileContext::setByteCodeGenerator(CodeGen::IRGenerator& generator)
{
   mpByteCodeGenerator = &generator;
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

bool CompileContext::loadClass(const String& classname)
{
   String lowercasename = classname;
   lowercasename.toLower();

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
   for ( ; it != mClasses.end(); ++it )
   {
      ASTClass* pclass = it->second;
      if ( pclass->getState() == ASTClass::eLoaded )
      {
         classes.push_back(pclass);
      }
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
   return it != mClasses.end() ? it->second : NULL;
}

const ASTClass& CompileContext::resolveClass(const String& classname) const
{
   return const_cast<CompileContext*>(this)->resolveClass(classname);
}

ASTClass& CompileContext::resolveClass(const String& classname)
{
   ASTClass* pclass = findClass(classname);
   if ( pclass == NULL )
   {
      throw new ClassNotFoundException(classname);
   }
   return *pclass;
}
