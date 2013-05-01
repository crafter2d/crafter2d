
#ifndef COMPILER_H_
#define COMPILER_H_

#include "script/script_base.h"

#include <vector>

#include "core/string/string.h"

#include "compilecontext.h"

namespace ByteCode
{
   class IRGenerator;
   class Program;
}

class AntlrStream;
class ASTNode;
class CompileStep;
class CompileCallback;
class CompileException;
class ClassRegistry;

class SCRIPT_API Compiler
{
   typedef std::vector<CompileStep*> Steps;
   typedef std::vector<ASTClass*> ASTClasses;

public:
   Compiler();

 // get/set
   bool             hasCallback() const;
   CompileCallback& getCallback();
   void             setCallback(CompileCallback& callback);

   void             setByteCodeGenerator(ByteCode::IRGenerator* pgenerator);
   void             setProgram(ByteCode::Program& program);

   const Literal& lookupLiteral(int index) const;

 // operations
   const ClassRegistry& getClassRegistry() const;
   void                 setClassRegistry(const ClassRegistry& registry);

 // compilation
   bool compile(const String& classname);
   void cleanUp();

private:
   friend class CompileContext;
   friend class PreloadVisitor;

   enum Phase { ePreload, eCompile };

 // operations
   void createLoadSteps();
   void createPrecompileSteps();
   void createCompileSteps();
   bool performSteps(ASTNode& node, Steps& steps);
   void sort(ASTClasses& classes, ASTClasses& sorted);

   bool load(const String& classname);
   void save(const ASTClass& ast);

   void reportError(CompileException& exception);
   void displayErrors(const String& currentfile);

   CompileContext    mContext;
   CompileCallback*  mpCallback;
   Steps             mLoadSteps;
   Steps             mPrecompileSteps;
   Steps             mCompileSteps;
   Phase             mPhase;
};

#endif // COMPILER_H_
