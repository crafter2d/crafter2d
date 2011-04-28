
#ifndef COMPILER_H_
#define COMPILER_H_

#include <string>
#include <vector>

#include "compilecontext.h"

class AntlrStream;
class ASTRoot;
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
 // operations
   void createSteps();
   bool performSteps(ASTRoot& root);

   CompileContext    mContext;
   CompileCallback*  mpCallback;
   Steps             mSteps;
};

#endif // COMPILER_H_
