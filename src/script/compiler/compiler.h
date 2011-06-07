
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
   friend class PreloadVisitor;

   enum Phase { ePreload, eCompile };

 // operations
   void createLoadSteps();
   void createPrecompileSteps();
   void createCompileSteps();
   bool performSteps(ASTNode& node, Steps& steps);

   bool load(const std::string& classname);
   void save(ASTClass& ast);

   void displayErrors(const std::string& currentfile);

   CompileContext    mContext;
   CompileCallback*  mpCallback;
   Steps             mLoadSteps;
   Steps             mPrecompileSteps;
   Steps             mCompileSteps;
   Phase             mPhase;
};

#endif // COMPILER_H_
