/***************************************************************************
*   Copyright (C) 2016 by Jeroen Broekhuizen                              *
*   crafter2d@outlook.com                                                 *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU Library General Public License as       *
*   published by the Free Software Foundation; either version 2 of the    *
*   License, or (at your option) any later version.                       *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU Library General Public     *
*   License along with this program; if not, write to the                 *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/
#include "yuiscriptregistration.h"

#include <memory>

#include "core/content/contentmanager.h"
#include "core/script/scriptcall.h"
#include "core/script/scriptmacros.h"
#include "core/script/scriptmanager.h"
#include "core/script/scriptobject.h"
#include "core/script/scriptregistrator.h"

#include "yuisystem.h"
#include "yuiwindow.h"

namespace c2d
{

void yui_init(ScriptCall& accessor)
{
   INIT_THIS(YuiSystem);
}

void yui_initialize(ScriptCall& accessor)
{
   GET_THIS(c2d::YuiSystem, system);

   ContentManager& contentmgr = accessor.get<ContentManager>(1);
   ScriptManager& scriptmgr = accessor.get<ScriptManager>(2);
   float width = accessor.getInt(3);
   float height = accessor.getInt(4);

   accessor.setResult(system.initialize(contentmgr, scriptmgr, width, height));
}

void yui_load(ScriptCall& accessor)
{
   GET_THIS(c2d::YuiSystem, system);

   const String& file = accessor.getString(1);
   c2d::YuiWindow* pwindow = system.load(file);

   RETURN_CLASS_OWNED(UTEXT("ui.YuiWindow"), pwindow);
}

void yui_setTheme(ScriptCall& accessor)
{
   GET_THIS(c2d::YuiSystem, system);

   const String& file = accessor.getString(1);
   system.setTheme(file);
}

void YuiRegisterScripts(ScriptManager & scriptmanager)
{
   std::unique_ptr<ScriptRegistrator> registrator(scriptmanager.getRegistrator());

   registrator->addClass(UTEXT("ui.YuiSystem"));
   registrator->addFunction(UTEXT("YuiSystem()"), yui_init);
   registrator->addFunction(UTEXT("initialize(engine.game.ContentManager, engine.game.ScriptManager, int, int)"), yui_initialize);
   registrator->addFunction(UTEXT("load(String)"), yui_load);
   registrator->addFunction(UTEXT("setTheme(String"), yui_setTheme);

   registrator->addClass(UTEXT("ui.YuiWindow"));

   registrator->registerCallbacks();
}

} // namespace c2d
