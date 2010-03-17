/***************************************************************************
 *   Copyright (C) 2009 by Jeroen Broekhuizen                              *
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
#ifndef KEY_EVENT_H
#define KEY_EVENT_H

#include "inputevent.h"

struct SDL_KeyboardEvent;

class KeyEvent : public InputEvent
{
public:
  // statics
   static KeyEvent FromSDL(const SDL_KeyboardEvent& event);

  // enums
   enum EventType {
      ePressed,
      eReleased
   };

   KeyEvent(int key, EventType type, int keymodifiers);
   virtual ~KeyEvent();

  // get/set
   int       getKey() const       { return mKey;  }
   EventType getEventType() const { return mType; }

private:
   int       mKey;
   EventType mType;
};

#endif
