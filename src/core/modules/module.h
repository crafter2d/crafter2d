
#ifndef MODULE_H
#define MODULE_H

#include "core/system/uuid.h"
#include "core/core_base.h"

namespace c2d
{
   class ModuleManager;

   // {371C78C7-3B7C-4E51-BF0B-354E9E44A470}
   static const Uuid UUID_GraphicsModule(0x371C78C7, 0x3B7C, 0x4E51, 0xBF0B, 0x354E9E44A470);
   // {CDD613E8-3EE9-4BA0-A67B-227B39F8AB28}
   static const Uuid UUID_InputModule(0xCDD613E8, 0x3EE9, 0x4BA0, 0xA67B, 0x227B39F8AB28);
   // {037F06A9-1388-43B8-B828-383B5B03EF5E}
   static const Uuid UUID_ScriptModule(0x037F06A9, 0x1388, 0x43B8, 0xB828, 0x383B5B03EF5E);
   // {E6A027CB-1553-4B7F-86EA-8E8D503118AC}
   static const Uuid UUID_SoundModule(0xE6A027CB, 0x1553, 0x4B7F, 0x86EA, 0x8E8D503118AC);
   // {611DCFE7-082D-4B32-B3A4-5235A733B0FF}
   static const Uuid UUID_WindowModule(0x611DCFE7, 0x082D, 0x4B32, 0xB3A4, 0x5235A733B0FF);

   enum ModuleKind
   {
      eGraphicsModule,
      eInputModule,
      eScriptModule,
      eContentModule,
      eSoundModule,
      eExtensionModule,
      eWindowModule,
   };

   class CORE_API Module
   {
   public:
      virtual ~Module();

    // get/set
      ModuleManager& getModuleManager();
      ModuleKind     getKind() const;
      const Uuid&    getUuid() const;

    // operations
      virtual void initialize();
      
   protected:
    // constructors
      Module(ModuleKind kind, const Uuid& uuid);

    // operations
      Module& lookupModule(const Uuid& uuid);

   private:
      friend class ModuleManager;

    // get/set
      void setModuleManager(ModuleManager& manager);

    // data
      ModuleManager* mpManager;
      ModuleKind     mKind;
      Uuid           mUuid;
   };
}

#endif // MODULE_H
