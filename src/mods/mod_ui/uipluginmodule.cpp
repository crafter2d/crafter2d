
#include "uipluginmodule.h"

#include "core/modules/graphicsmodule.h"
#include "core/modules/inputmodule.h"
#include "core/modules/scriptmodule.h"
#include "core/modules/modulemanager.h"

#include "lookandfeel/default/defaultlookandfeel.h"
#include "ui/system.h"

using namespace c2d;

namespace ui
{
   UiPluginModule::UiPluginModule(const c2d::Uuid& uuid) :
      PluginModule(uuid)
   {
   }

   // - Implementation

   void UiPluginModule::initialize()
   {
      GraphicsModule& graphicsmod = static_cast<GraphicsModule&>(lookupModule(c2d::UUID_GraphicsModule));
      InputModule& inputmod = static_cast<InputModule&>(lookupModule(c2d::UUID_InputModule));
      ScriptModule& scriptmod = static_cast<ScriptModule&>(lookupModule(c2d::UUID_ScriptModule));

      System& system = System::createSystem(graphicsmod.getDevice());
      system.setLookAndFeel(new DefaultLookAndFeel());
   }
}
