
#ifndef PLUGIN_MODULE_H
#define PLUGIN_MODULE_H

#include "core/modules/module.h"
#include "core/core_base.h"

namespace c2d
{
   class Plugin;

   class CORE_API PluginModule : public Module
   {
   public:
      PluginModule(Plugin* pplugin);
      virtual ~PluginModule();

    // get/set
      Plugin& getPlugin();

   private:

    // data
      Plugin* mpPlugin; // owns
   };
}

#endif // PLUGIN_MODULE_H
