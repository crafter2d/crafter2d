// mod_effect.cpp : Defines the exported functions for the DLL application.
//

#include "ui_base.h"

#include "core/modules/uimodule.h"

#include "yauisystem.h"

using namespace c2d;

extern "C" MOD_API Module* MOD_DECL getModule()
{
   return new UiModule(new YaUiSystem());
}

