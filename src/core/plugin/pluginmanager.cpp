
#include "pluginmanager.h"

#include "core/system/exception.h"

namespace c2d
{
   PluginManager::PluginManager()
   {
   }

   // - Query

   System& PluginManager::querySystem(SystemKind kind)
   {
      throw c2d::Exception(UTEXT("not implemented yet!"));
   }
}