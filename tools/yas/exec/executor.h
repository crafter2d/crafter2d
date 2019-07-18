#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <core/commandline/commandline.h>
#include <core/modules/modulemanager.h>

namespace c2d
{
   class ScriptManager;
}

class Executor
{
public:
   Executor(int argc, char *argv[]);

 // operations
   void exec();

private:

 // operations
   c2d::ScriptManager& loadScriptManager();

 // data members
   c2d::ModuleManager mModuleMgr;
   CommandLine   mCommandLine;
};

#endif // EXECUTOR_H
