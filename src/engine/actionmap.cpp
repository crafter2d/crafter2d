/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
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
#include "actionmap.h"

#include "core/script/scriptobject.h"
#include "core/script/scriptmanager.h"

#include "net/events/actionevent.h"

#include "client.h"

namespace c2d
{

   ActionMap::ActionMap() :
      mpProcess(nullptr),
      mpScript(nullptr),
      mActions()
   {
   }

   ActionMap::~ActionMap()
   {
      mpProcess = nullptr;
   }

   // - Get/set

   void ActionMap::setProcess(Process& process)
   {
      mpProcess = &process;

      mpScript = process.getScriptManager().load(UTEXT("engine.game.ActionMap"), this, false);
      ASSERT_PTR(mpScript);
   }

   // - Operations

   void ActionMap::bind(int action, const String& function)
   {
      mActions[action] = function;
   }

   void ActionMap::process(int action, bool down, float delta)
   {
      Actions::const_iterator it = mActions.find(action);
      if ( it == mActions.end() )
      {
         ActionEvent event(action, down);
         c2d::Client& client = static_cast<c2d::Client&>(getProcess());
         client.sendToServer(event);
      }
      else
      {
         const String& function = it->second;

         ASSERT_PTR(mpScript);
         mpScript->prepareCall(2);
         mpScript->arg(0, down);
         mpScript->arg(1, delta);
         mpScript->call(function);
      }
   }

   void ActionMap::processRemote(const ActionEvent& event, Entity& object)
   {
      ASSERT_PTR(mpScript);

      mpScript->prepareCall(2);
      mpScript->arg(0, &object);
      mpScript->arg(1, event.getAction());

      if ( event.isDown() )
         mpScript->call(UTEXT("onKeyDown"));
      else
         mpScript->call(UTEXT("onKeyUp"));
   }

} // namespace c2d
