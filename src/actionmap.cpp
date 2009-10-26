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

#include "game.h"
#include "script.h"
#include "scriptmanager.h"
#include "object.h"

IMPLEMENT_REPLICATABLE(InputEventId, InputEvent, NetEvent)

InputEvent::InputEvent(): 
   NetEvent(inputEvent),
   action(none),
   down(false)
{
}

InputEvent::InputEvent(Action act, bool dwn): 
   NetEvent(inputEvent), 
   action(act), 
   down(dwn)
{
}

void InputEvent::pack(BitStream& stream) const
{
   NetEvent::pack(stream);
   stream << action << down;
}

void InputEvent::unpack(BitStream& stream)
{
   NetEvent::unpack(stream);
   stream >> (int&)action >> down;
}

/******************************************************
 * KeyMap class
 */

void ActionMap::bind(int key, const char* function)
{
   actions[(Action)key] = function;
}

bool ActionMap::process (int key, bool down)
{
   Action action = (Action)key;
   if (actions.find(action) == actions.end())
      return false;

   Script& script = ScriptManager::getInstance().getTemporaryScript();
   script.prepareCall (actions[action]);
   script.addParam ((int)down);
   script.run(1);
   return true;
}

bool ActionMap::process(const InputEvent& event, Object* obj)
{
   Action action = event.getAction();
   if (actions.find(action) == actions.end())
      return false;

   Script& script = ScriptManager::getInstance().getTemporaryScript();
   script.prepareCall (actions[action]);
   script.addParam(obj, "Object");
   script.addParam((int)event.isDown());
   script.run(2);
   return true;
}

/******************************************************
 * KeyMap class
 */

void KeyMap::bind(int key, int action, bool local)
{
   KeyInfo info;
   info.action = (Action)action;
   info.local  = local;

   keys[key] = info;
}

bool KeyMap::process(int key, bool down)
{
   // get the action for this key
   if (keys.find(key) == keys.end())
      return false;
   int action = keys[key].action;

   if (keys[key].local)
   {
      // process action local
      ActionMap* map = Game::getInstance().getClient().getActionMap();
      if (map) map->process(action, down);
   }
   else {
      // set up an input event
      BitStream stream;
      InputEvent event(static_cast<Action>(action), down);
      stream << &event;

      // send the event to the server
      NetConnection* conn = Game::getInstance().getClient().getConnection();
      conn->send(&stream);
   }
   return true;
}
