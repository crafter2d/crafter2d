
#ifndef COMPILER_H_
#define COMPILER_H_

#include <vector>
#include <map>

#include "core/commandline/commandline.h"
#include "core/string/string.h"

#include "compilecontext.h"

class ASTNode;
class ASTRoot;
class CompileStep;
class CompileException;
class File;

class Compiler
{
   typedef std::vector<CompileStep*> Steps;
   typedef std::vector<ASTClass*> ASTClasses;
   typedef std::map<String, ASTRoot*> FileLookup;

public:
   Compiler(int argc, char* parv[]);
   ~Compiler();

 // execution
   int exec();

private:
   friend class CompileContext;
   friend class PreloadVisitor;

   enum Phase { ePreload, eCompile };

 // compilation
   bool compile(const String& classname);

 // operations
   void createCompileSteps();
   void cleanUp();

   bool performSteps(ASTNode& node, Steps& steps);
   
   ASTRoot& load(const String& classname);
   ASTRoot& load(File& file);
   void     save(ASTClass& ast);

 // error reporting
   void reportError(CompileException& exception);
   void displayErrors(const String& currentfile);

 // data
   CommandLine       mCommands;
   CompileContext    mContext;
   FileLookup        mFileLookup;
   Steps             mCompileSteps;
   Phase             mPhase;
   String            mOutputDir;
};

#endif // COMPILER_H_
