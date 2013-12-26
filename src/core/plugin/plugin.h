
#ifndef PLUGIN_H
#define PLUGIN_H

#include "core/core_base.h"

namespace c2d
{
   class PluginManager;

   class CORE_API Plugin
   {
   public:
      Plugin();

    // get/set
      bool           hasManager() const;
      PluginManager& getManager();
      void           setManager(PluginManager& manager);

   private:

    // data
      PluginManager* mpManager;
   };
}

#endif // PLUGIN_H
