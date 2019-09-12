// mod_effect.cpp : Defines the exported functions for the DLL application.
//

#include "ui_base.h"

#include <memory>

#include "core/defines.h"
#include "core/modules/moduleinfo.h"
#include "core/modules/module.h"
#include "core/modules/modulemanager.h"
#include "core/modules/contentmodule.h"
#include "core/modules/scriptmodule.h"
#include "core/vfs/filesystem.h"

#include "content/theme/themereader.h"
#include "content/theme/themewriter.h"

#include "yuiscriptregistration.h"

using namespace c2d;

// {469939F4-52C0-414F-ABAD-F92727489500}
static const Uuid YuiModuleUUID(0x469939F4, 0x52C0, 0x414f, 0xABAD, 0xF92727489500);

class YuiModule : public c2d::Module
{
public:
   YuiModule() : Module(ModuleKind::eExtensionModule, YuiModuleUUID) {}

   void initialize() override
   {
      FileSystem::getInstance().addPath(UTEXT("mod_ui.zip"));

      ScriptModule* pscriptmodule = static_cast<ScriptModule*>(getModuleManager().lookup(UUID_ScriptModule));
      ASSERT_PTR(pscriptmodule);
      YuiRegisterScripts(pscriptmodule->getManager());
   }
};

// {959A9142-151B-4994-AC15-729E118CF102}
static const Uuid ThemeUUID(0x959A9142, 0x151B, 0x4994, 0xAC15, 0x729E118CF102);

extern "C" MOD_API ModuleInfo* MOD_DECL getModuleInfo()
{
   return ModuleInfo::create(1, { UUID_ScriptModule });
}

extern "C" MOD_API void MOD_DECL freeModuleInfo(ModuleInfo* pinfo)
{
   ModuleInfo::free(pinfo);
}

extern "C" MOD_API Modules* MOD_DECL getModules()
{
   auto pthememodule = std::make_unique<ContentModule>(ThemeUUID);
   pthememodule->setSupportedFiles(UTEXT("theme"));
   pthememodule->setReader(new ThemeReader());
   pthememodule->setWriter(new ThemeWriter());

   auto pmodule = new YuiModule();

   return Modules::create({ pmodule, pthememodule.release() });
}

extern "C" MOD_API void MOD_DECL freeModules(Modules* pmodules)
{
   ASSERT(pmodules->count == 2);
   Modules::free(pmodules);
}
