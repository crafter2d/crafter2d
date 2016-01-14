
#include "uimodule.h"

c2d::UiModule::UiModule(UiSystem* psystem) :
   Module(ModuleKind::eUiModule, UUID_UiModule),
   mpSystem(psystem)
{
}

c2d::UiModule::~UiModule()
{
}
