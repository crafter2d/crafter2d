
#include "pluginmodule.h"

#include "core/plugin/plugin.h"
#include "core/defines.h"

// Does not have the uuid itself, should be given by the plugin implementation

namespace c2d
{
   PluginModule::PluginModule(const Uuid& uuid) :
      Module(ePluginModule, uuid)
   {
   }
}
