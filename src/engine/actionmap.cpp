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
#ifndef JENGINE_INLINE
#  include "actionmap.inl"
#endif

#include "net/events/actionevent.h"

#include "engine/script/script.h"
#include "engine/script/scriptmanager.h"

#include "client.h"

ActionMap::ActionMap():
   mpProcess(NULL),
   mpScript(NULL),
   mActions()
{
}

ActionMap::~ActionMap()
{
   mpProcess = NULL;
}

// - Get/set

void ActionMap::setProcess(Process& process)
{
   mpProcess = &process;

   mpScript = process.getScriptManager().loadNative("ActionMap", this, false);
   ASSERT_PTR(mpScript);
}

// - Operations

void ActionMap::bind(int action, const char* function)
{
   mActions[action] = function;
}

void ActionMap::process(int action, bool down)
{
   Actions::const_iterator it = mActions.find(action);
   if ( it == mActions.end() )
   {
      ActionEvent event(action, down);
      Client& client = dynamic_cast<Client&>(getProcess());
      client.sendToServer(event);
   }
   else
   {
      std::string function = it->second;
      
      ASSERT_PTR(mpScript);
      mpScript->addParam(down);
      mpScript->run(function);
   }
}

void ActionMap::processRemote(const ActionEvent& event, Actor& object)
{
   ASSERT_PTR(mpScript);
   mpScript->addParam("Actor", &object);
   mpScript->addParam(event.getAction());

   if ( event.isDown() )
      mpScript->run("onKeyDown");
   else
      mpScript->run("onKeyUp");
}
