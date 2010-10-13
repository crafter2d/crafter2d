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

#include "client.h"
#include "script.h"
#include "scriptmanager.h"
#include "object.h"
#include "process.h"

ActionMap::ActionMap():
   mActions()
{
}

ActionMap::~ActionMap()
{
   mpProcess = NULL;
}

// - operations

void ActionMap::bind(int action, const char* function)
{
   mActions[action] = function;
}

void ActionMap::process(int action, bool down)
{
   Client& client = dynamic_cast<Client&>(getProcess());

   Actions::const_iterator it = mActions.find(action);
   if ( it == mActions.end() )
   {
      ActionEvent event(action, down);
      client.sendToServer(event);
   }
   else
   {
      const char* pfunction = it->second;

      Script& script = client.getScriptManager().getTemporaryScript();
      script.prepareCall (pfunction);
      script.addParam(down);
      script.run(1);
   }
}

void ActionMap::processRemote(const ActionEvent& event, Object& object)
{
   int action = event.getAction();
   Actions::const_iterator it = mActions.find(action);
   if ( it != mActions.end() )
   {
      Script& script = mpProcess->getScriptManager().getTemporaryScript();
      script.prepareCall(it->second);
      script.addParam(&object, "Object");
      script.addParam(event.isDown());
      script.run(2);
   }
}
