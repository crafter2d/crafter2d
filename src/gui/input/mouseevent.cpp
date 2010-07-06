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

#include "mouseevent.h"

#include <SDL/SDL.h>

//-----------------------------------
// - Statics
//-----------------------------------

MouseEvent::Button MouseEvent::toMouseEventButton(int sdlbutton)
{
   switch ( sdlbutton )
   {
      case SDL_BUTTON_LEFT:      return eLeft;
      case SDL_BUTTON_RIGHT:     return eRight;
      case SDL_BUTTON_MIDDLE:    return eMiddle;
      case SDL_BUTTON_WHEELUP:   return eWheelUp;
      case SDL_BUTTON_WHEELDOWN: return eWheelDown;
   }

   return eInvalid;
}

MouseEvent MouseEvent::FromSDL(SDL_MouseButtonEvent& event)
{
   Point location(event.x, event.y);
   Button button = toMouseEventButton(event.button);
   int keymodifiers = getModifiers();

   EventType eventtype;
   switch ( event.type )
   {
      case SDL_MOUSEBUTTONDOWN:
         eventtype = ePressed;
         break;
      case SDL_MOUSEBUTTONUP:
         eventtype = eReleased;
         break;
   }

   return MouseEvent(button, eventtype, keymodifiers, location);
}

MouseEvent MouseEvent::FromSDL(SDL_MouseMotionEvent& event)
{
   int buttons = 0;
   if ( event.state & SDL_BUTTON(SDL_BUTTON_LEFT) )
      SET_FLAG(buttons, eLeft);
   if ( event.state & SDL_BUTTON(SDL_BUTTON_RIGHT) )
      SET_FLAG(buttons, eRight);
   if ( event.state & SDL_BUTTON(SDL_BUTTON_MIDDLE) )
      SET_FLAG(buttons, eMiddle);

   int keymodifiers = getModifiers();
   Point location(event.x, event.y);
   Point relative(event.xrel, event.yrel);

   return MouseEvent(buttons, eMotion, keymodifiers, location, relative);
}

//-----------------------------------
// - Construction
//-----------------------------------

MouseEvent::MouseEvent(int buttons, EventType eventtype, int keymodifiers, const Point& location, const Point& relative):
   InputEvent(keymodifiers),
   mButtons(buttons),
   mEventType(eventtype),
   mLocation(location),
   mRelative(relative)
{
}

MouseEvent::~MouseEvent()
{
}
