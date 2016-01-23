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
#ifndef _ACTIONMAP_H
#define _ACTIONMAP_H

#include <map>

#include "core/string/string.h"

#include "net/events/actionevent.h"

class ActionEvent;
class Entity;

namespace c2d
{
   class Process;
   class ScriptObject;

   /// \brief The mapping of actions to Lua functions.
   ///
   /// An ActionMap is used to map actions to Lua functions. It can be assigned to both the client and
   /// the server processes. By adding actions to the ActionMap that is assigned to the client will run
   /// local functions (thus in the client Lua state). In case an action is not present an action message
   /// will be send to the server which then takes care of the action.
   ///
   /// Server side actions have a slightly different argument count than the client side actions.
   /// <code>function jumpAction(object, down)
   ///   -- do something
   /// end</code>
   /// And client side:
   /// <code>function screenshotAction(down)
   ///   -- take screenshot
   /// end</code>
   /// As you see there is no object available in the client side. To adjust the object, you need to
   /// define the action at the server side.

   class ActionMap
   {
   public:
      ActionMap();
      ~ActionMap();

      // get/set
      bool     hasProcess() const;
      Process& getProcess();
      void     setProcess(Process& process);

      // operations
      void bind(int action, const String& function);

      // processing
      void process(int action, bool down, float delta);
      void processRemote(const ActionEvent& event, Entity& object);

   private:
      typedef std::map<int, String> Actions;

      Process*       mpProcess;
      ScriptObject*  mpScript;
      Actions        mActions;
   };
}

#ifdef JENGINE_INLINE
#  include "actionmap.inl"
#endif

#endif
