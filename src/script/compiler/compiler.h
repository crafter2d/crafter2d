
#ifndef COMPILER_H_
#define COMPILER_H_

#include <string>
#include <vector>

#include "compilecontext.h"

class AntlrStream;
class ASTNode;
class CompileStep;
class CompileCallback;

class Compiler
{
   typedef std::vector<CompileStep*> Steps;
   typedef std::vector<std::string> Files;

public:
   Compiler();

 // get/set
   bool             hasCallback() const;
   CompileCallback& getCallback();
   void             setCallback(CompileCallback& callback);

   const Literal& lookupLiteral(int index) const;

 // compilation
   bool compile(const std::string& classname);

private:
   friend class CompileContext;

   enum Phase { eLoad, eLoading, eCompile };

 // operations
   void createLoadSteps();
   void createCompileSteps();

   void displayErrors(const std::string& currentfile);
   bool loadClass(const std::string& classname);

   void save(ASTClass& ast);

   bool performSteps(ASTNode& node, Steps& steps);

   CompileContext    mContext;
   CompileCallback*  mpCallback;
   Steps             mLoadSteps;
   Steps             mSteps;
   Files             mFiles;
   Phase             mPhase;
};

#endif // COMPILER_H_
