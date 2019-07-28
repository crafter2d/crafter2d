
#include "executor.h"

#include <core/modules/modulemanager.h>
#include <core/modules/scriptmodule.h>
#include <core/script/scriptmanager.h>
#include <core/script/scriptobject.h>
#include <core/script/scriptobjecthandle.h>
#include <core/vfs/filesystem.h>

using namespace c2d;

Executor::Executor(int argc, char *argv[]) :
   mModuleMgr(),
   mCommandLine(argc, argv)
{
}

void Executor::exec()
{
   auto& scriptmanager = loadScriptManager();

   FileSystem& fs = FileSystem::getInstance();
   fs.removeAll();
   fs.addPath(UTEXT("data.zip/scripts"));

   auto ppath = mCommandLine.getArgument(UTEXT("p"));
   if ( ppath )
      fs.addPath(ppath->getValue());
   else
      fs.addPath(UTEXT("."));

   // last entry should be the runnable class & function
   String runnable = mCommandLine[mCommandLine.size() - 1].getName();
   ScriptObjectHandle object(scriptmanager.load(runnable));
   if ( object )
   {
      object->call(UTEXT("run"));
   }
}

c2d::ScriptManager& Executor::loadScriptManager()
{
   if ( !mModuleMgr.initialize() )
   {
      throw std::exception("Could not initialize the module manager");
   }

   c2d::Module* pmodule = mModuleMgr.lookup(c2d::UUID_ScriptModule);
   if ( pmodule == nullptr )
   {
      // no need to continue testing as it seems there is no script module available
      throw std::exception("Could not load any script manager");
   }

   ScriptModule* pmod = static_cast<ScriptModule*>(pmodule);
   ScriptManager& scriptmanager = pmod->getManager();
   return scriptmanager;
}
