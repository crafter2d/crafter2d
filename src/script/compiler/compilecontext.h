
#ifndef COMPILE_CONTEXT_H_
#define COMPILE_CONTEXT_H_

#include <map>
#include <string>

#include "script/ast/asttype.h"

#include "script/common/literaltable.h"

#include "compilelog.h"

class Compiler;
class ASTClass;
class VirtualClass;

class CompileContext
{
   typedef std::map<std::string, ASTClass*> ClassMap;

public:
   explicit CompileContext(Compiler& compiler);

 // get/set
   CompileLog& getLog();

   const LiteralTable& getLiteralTable() const;
         LiteralTable& getLiteralTable();

   VirtualClass* getResult();
   void          setResult(VirtualClass* pclass);
   
 // query
   bool hasClass(const std::string& classname) const;

 // operations
   void addClass(ASTClass* pclass);
   bool loadClass(const std::string& classname);

   void collectCompileClasses(std::vector<ASTClass*>& classes);

 // search
   const ASTClass* findClass(const std::string& classname) const;
         ASTClass* findClass(const std::string& name);

   const ASTClass& resolveClass(const std::string& classname) const;
         ASTClass& resolveClass(const std::string& classname);

private:
   void insertInternalTypes();

   Compiler&      mCompiler;
   ClassMap       mClasses;
   LiteralTable   mLiteralTable;
   CompileLog     mLog;
   VirtualClass*  mpResult;
};

#endif // COMPILE_CONTEXT_H_
