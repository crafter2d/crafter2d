
#ifndef COMPILE_CONTEXT_H_
#define COMPILE_CONTEXT_H_

#include <map>

#include "core/string/string.h"

#include "yasc/common/literaltable.h"

#include "compileinterfacetable.h"
#include "compilelog.h"

class ASTClass;
class ASTFunction;
class Compiler;

class CompileContext
{
   typedef std::map<String, ASTClass*> ClassMap;

public:
   explicit CompileContext(Compiler& compiler);
           ~CompileContext();

 // get/set
   CompileLog& getLog();

   const LiteralTable& getLiteralTable() const;
         LiteralTable& getLiteralTable();

   Compiler& getCompiler();
   
 // query
   bool hasClass(const String& classname) const;

 // operations
   void addClass(ASTClass* pclass);
   void addInterface(ASTClass& interfce);

   bool loadClass(const String& classname);
   
 // search
   const ASTClass* findClass(const String& classname) const;
         ASTClass* findClass(const String& name);

   const ASTClass& resolveClass(const String& classname) const;
         ASTClass& resolveClass(const String& classname);

   const ASTFunction& resolveFunction(const String& call) const;

private:
 // operations
   void insertInternalTypes();
   void loadBaseAndInterfaces(ASTClass* pclass);

 // data
   Compiler&               mCompiler;
   ClassMap                mClasses;
   CompileInterfaceTable   mInterfaces;
   LiteralTable            mLiteralTable;
   CompileLog              mLog;
};

#endif // COMPILE_CONTEXT_H_
