/***************************************************************************
 *   Copyright (C) 2011 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
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
#include "script/vm/virtualnativeobject.h"
#include "script/vm/virtualmachine.h"
#include "script/vm/virtualobjectreference.h"

#include "engine/script/scriptobject.h"
#include "engine/script/scriptmanager.h"

#include "game.h"

void Game_getWindowFactory(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Game* pgame = (Game*) thisobject->asNative().getObject();

   GameWindowFactory& factory = pgame->getWindowFactory();
   VirtualObjectReference ref = machine.instantiateNative("GameWindowFactory", &factory);
   ref->asNative().setOwned(false);

   accessor.setResult(ref);
}

void Game_setActive(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObjectReference& thisobject = accessor.getThis();
   Game* pgame = (Game*) thisobject->asNative().getObject();

   bool active = accessor.getBoolean(1);

   pgame->setActive(active);
}

// - Registration

void script_game_register(ScriptManager& manager)
{
   ScriptRegistrator registrator;

   registrator.addCallback("Game_getWindowFactory", Game_getWindowFactory);
   registrator.addCallback("Game_setActive", Game_setActive);

   registrator.registerCallbacks(manager);
}
