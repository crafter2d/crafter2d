
#ifndef UI_PLUGIN_H
#define UI_PLUGIN_H

#include "core/modules/pluginmodule.h"

namespace c2d
{
   class Module;
}

namespace ui
{
   class UiPluginModule : public c2d::PluginModule
   {
   public:
      UiPluginModule(const c2d::Uuid& uuid);

    // implementation
      virtual void initialize() override;
   };
}

#endif // UI_PLUGIN_H
