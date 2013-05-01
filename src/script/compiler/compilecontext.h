
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
class VirtualClass;

namespace ByteCode
{
   class IRGenerator;
}

class CompileContext
{
   typedef std::map<String, ASTClass*> ClassMap;
   typedef std::map<String, VirtualClass*> VirtualClassMap;

public:
   explicit CompileContext(Compiler& compiler);
           ~CompileContext();

 // get/set
   CompileLog& getLog();

   StringCache& getStringCache();

   const LiteralTable& getLiteralTable() const;
         LiteralTable& getLiteralTable();

   const ClassRegistry& getClassRegistry() const;
   void                 setClassRegistry(const ClassRegistry& registry);

   ByteCode::IRGenerator& getByteCodeGenerator();
   void                  setByteCodeGenerator(ByteCode::IRGenerator* pgenerator);

   ByteCode::Program& getProgram();
   void               setProgram(ByteCode::Program& program);
   
 // query
   bool hasClass(const String& classname) const;

 // operations
   void addClass(ASTClass* pclass);
   bool loadClass(const String& classname);

   void addVirtualClass(VirtualClass* pclass);

   void collectCompileClasses(std::vector<ASTClass*>& classes);

 // search
   const ASTClass* findClass(const String& classname) const;
         ASTClass* findClass(const String& name);

   const ASTClass& resolveClass(const String& classname) const;
         ASTClass& resolveClass(const String& classname);

   const ASTFunction& resolveFunction(const String& call) const;

   VirtualClass& resolveVirtualClass(const String& classname);

private:
   void insertInternalTypes();

   Compiler&               mCompiler;
   ClassMap                mClasses;
   VirtualClassMap         mVirtualClasses;
   ClassRegistry           mClassRegistry;
   ByteCode::IRGenerator*  mpByteCodeGenerator; // owns
   StringCache             mStringCache;
   LiteralTable            mLiteralTable;
   ByteCode::Program*      mpProgram;
   CompileLog              mLog;
};

#endif // COMPILE_CONTEXT_H_
