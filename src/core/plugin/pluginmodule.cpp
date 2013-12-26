
#include "pluginmodule.h"

#include "core/plugin/plugin.h"
#include "core/defines.h"

namespace c2d
{
   PluginModule::PluginModule(Plugin* pplugin):
      Module(ePlugin),
      mpPlugin(pplugin)
   {
   }

   PluginModule::~PluginModule()
   {
      delete mpPlugin;
      mpPlugin = NULL;
   }

   // - Get/set

   Plugin& PluginModule::getPlugin()
   {
      ASSERT_PTR(mpPlugin);
      return *mpPlugin;
   }
}
