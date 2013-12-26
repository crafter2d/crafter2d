
#include "plugin.h"

#include "core/defines.h"

namespace c2d
{
   Plugin::Plugin():
      mpManager(NULL)
   {
   }

   // - Get/set

   bool Plugin::hasManager() const
   {
      return mpManager != NULL;
   }

   PluginManager& Plugin::getManager()
   {
      ASSERT_PTR(mpManager);
      return *mpManager;
   }
}