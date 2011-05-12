
#ifndef COMPILE_CONTEXT_H_
#define COMPILE_CONTEXT_H_

#include <map>
#include <string>
#include <vector>

#include "script/ast/asttype.h"

#include "script/common/literaltable.h"

#include "classresolver.h"
#include "compilelog.h"

class Compiler;
class ASTClass;
class VirtualClass;

class CompileContext
{
   typedef std::map<std::string, ASTClass*> Classes;

public:
   explicit CompileContext(Compiler& compiler);

 // get/set
   CompileLog& getLog();

   const LiteralTable& getLiteralTable() const;
         LiteralTable& getLiteralTable();

   VirtualClass* getResult();
   void          setResult(VirtualClass* pclass);

   void resetCollection();

 // query
   bool hasClass(const std::string& classname) const;

   std::string getFullName(const std::string& classname) const;

 // operations
   void addClass(ASTClass* pclass);
   void addPath(const std::string& path);

 // search
   const ASTClass* findClass(const std::string& classname) const;
         ASTClass* findClass(const std::string& name);

private:
   void insertInternalTypes();

   Compiler&      mCompiler;
   Classes        mClasses;
   ClassResolver  mResolver;
   LiteralTable   mLiteralTable;
   CompileLog     mLog;
   VirtualClass*  mpResult;
   bool           mCollect;
};

#endif // COMPILE_CONTEXT_H_
