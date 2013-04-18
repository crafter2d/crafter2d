
#ifndef COMPILE_CONTEXT_H_
#define COMPILE_CONTEXT_H_

#include <map>

#include "core/string/string.h"

#include "script/ast/asttype.h"
#include "script/common/stringcache.h"
#include "script/common/literaltable.h"
#include "script/common/classregistry.h"

#include "compilelog.h"

class ASTClass;
class Compiler;

namespace CodeGen
{
   class IRGenerator;
}

class CompileContext
{
   typedef std::map<String, ASTClass*> ClassMap;

public:
   explicit CompileContext(Compiler& compiler);

 // get/set
   CompileLog& getLog();

   StringCache& getStringCache();

   const LiteralTable& getLiteralTable() const;
         LiteralTable& getLiteralTable();

   const ClassRegistry& getClassRegistry() const;
   void                 setClassRegistry(const ClassRegistry& registry);

   CodeGen::IRGenerator& getByteCodeGenerator();
   void                  setByteCodeGenerator(CodeGen::IRGenerator& generator);
   
 // query
   bool hasClass(const String& classname) const;

 // operations
   void addClass(ASTClass* pclass);
   bool loadClass(const String& classname);

   void collectCompileClasses(std::vector<ASTClass*>& classes);

 // search
   const ASTClass* findClass(const String& classname) const;
         ASTClass* findClass(const String& name);

   const ASTClass& resolveClass(const String& classname) const;
         ASTClass& resolveClass(const String& classname);

private:
   void insertInternalTypes();

   Compiler&      mCompiler;
   ClassMap       mClasses;
   ClassRegistry  mClassRegistry;
   CodeGen::IRGenerator*   mpByteCodeGenerator;
   StringCache    mStringCache;
   LiteralTable   mLiteralTable;
   CompileLog     mLog;
};

#endif // COMPILE_CONTEXT_H_
