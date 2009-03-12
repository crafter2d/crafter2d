/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _ACTIONMAP_H
#define _ACTIONMAP_H

#include <map>
#include "net/netevent.h"

class Object;

enum Action {
   moveLeft,
   moveRight,
   moveUp,
   moveDown,

   jump,

   runFunction,
   swapConsole,
   none,
   quit
};

/// InputEvent
class InputEvent: public NetEvent
{
public:
   DEFINE_REPLICATABLE(InputEvent)

                  InputEvent();
                  InputEvent(Action act, bool dwn);

           Action getAction() const;
           bool   isDown() const;

   virtual void   pack(BitStream& stream) const;
   virtual void   unpack(BitStream& stream);

protected:
   Action action;
   bool down;
};

/// ActionMap
/// Server-side action map to perform the actual actions
class ActionMap
{
public:
   void bind(int key, const char* function);

   bool process(int key, bool down=true);
   bool process(InputEvent& event, Object* obj);

private:
   std::map<Action, const char*> actions;
};

/// KeyMap
/// Client-side mapping of the keys to actions
class KeyMap
{
public:
   void bind(int key, int action, bool local=false);

   bool process(int key, bool down=true);

private:
   struct KeyInfo {
      int action;
      bool local;
   };

   std::map<int, KeyInfo> keys;
};

#ifdef JENGINE_INLINE
#  include "actionmap.inl"
#endif

#endif
