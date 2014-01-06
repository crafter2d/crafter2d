
#ifndef PLUGIN_MODULE_H
#define PLUGIN_MODULE_H

#include "module.h"

namespace c2d
{
   class CORE_API PluginModule : public Module
   {
   protected:
      explicit PluginModule(const Uuid& uuid);

   private:

    // data
   };
}

#endif // PLUGIN_MODULE_H
