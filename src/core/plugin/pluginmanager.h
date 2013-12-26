
#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

namespace c2d
{
   class System;

   class PluginManager
   {
   public:
      enum SystemKind
      {
         eRenderSystem,
         eScriptSystem,
         eFileSystem
      };

      PluginManager();

    // query
      System& querySystem(SystemKind kind);
   };
}

#endif // PLUGIN_MANAGER_H
