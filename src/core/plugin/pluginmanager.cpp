
#include "pluginmanager.h"

#include <exception>

namespace c2d
{
   PluginManager::PluginManager()
   {
   }

   // - Query

   System& PluginManager::querySystem(SystemKind kind)
   {
      throw std::exception("not implemented yet!");
   }
}