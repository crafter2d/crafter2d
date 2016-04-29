
#include "uimodule.h"

#include "core/ui/uisystem.h"

c2d::UiModule::UiModule(UiSystem* psystem) :
   Module(ModuleKind::eUiModule, UUID_UiModule),
   mSystem(psystem)
{
}

c2d::UiModule::~UiModule()
{
}
