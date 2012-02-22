
#ifndef COMPILER_H_
#define COMPILER_H_

#include "script/script_base.h"

#include <string>
#include <vector>

#include "compilecontext.h"

class AntlrStream;
class ASTNode;
class CompileStep;
class CompileCallback;
class CompileException;

class SCRIPT_API Compiler
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

   typedef std::vector<ASTClass*> ASTClasses;

   enum Phase { ePreload, eCompile };

 // operations
   void createLoadSteps();
   void createPrecompileSteps();
   void createCompileSteps();
   bool performSteps(ASTNode& node, Steps& steps);
   void sort(ASTClasses& classes, ASTClasses& sorted);

   bool load(const std::string& classname);
   void save(ASTClass& ast);

   void reportError(CompileException& exception);
   void displayErrors(const std::string& currentfile);

   CompileContext    mContext;
   CompileCallback*  mpCallback;
   Steps             mLoadSteps;
   Steps             mPrecompileSteps;
   Steps             mCompileSteps;
   Phase             mPhase;
};

#endif // COMPILER_H_
